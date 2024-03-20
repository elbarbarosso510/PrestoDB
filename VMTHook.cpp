 #include "VMTHook.h"
#include <Windows.h>
#include <intrin.h>

void* CodeJmpTo(void* ptr)
{
	BYTE runcode[] = { 0xFF,0x25 ,0x00,0x00,0x00,0x00 ,/*begin*/0,0,0,0,0,0,0,0 };
	*(UINT64*)&runcode[6] = (UINT64)ptr;
	return runcode;
}
void CodeJmpTo_1(void* from, void* to)
{
	char code[2] = { '\xEB',0};
	*(char*)&code[1] = (char)((((__int64)to - (__int64)from) - 2));
	memcpy(from, code, 2);
}
void CodeJmpTo_4(void* from, void* to)
{
	char code[5] = { '\xE9',0,0,0,0 };
	*(INT32*)&code[1] = (INT32)((((__int64)to - (__int64)from) - 5));
	memcpy(from, code, 5);
}

void CodeJmpTo_8(void* from, void* to)
{
	BYTE code[] = { 0xFF,0x25 ,0x00,0x00,0x00,0x00 ,/*begin*/0,0,0,0,0,0,0,0 };
	*(UINT64*)&code[6] = (UINT64)to;
	memcpy(from, code, 14);
}
VMTHook::VMTHook(void* object)
{
	this->m_ptr_object = object;
	this->m_ptr_object_vtable = *reinterpret_cast<uintptr_t**>(object);
	this->m_object_vtable_size = 0;
	this->m_object_vtable_size = this->get_function_count();
	this->m_ptr_proc_fake_table = VirtualAlloc(NULL, 16 * this->m_object_vtable_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	this->m_ptr_object_fake_vtable = new uintptr_t[this->m_object_vtable_size];
	for (size_t i = 0; i < this->m_object_vtable_size; i++)
	{
		memcpy((char*)this->m_ptr_proc_fake_table + (16 * i), CodeJmpTo((void*)this->m_ptr_object_vtable[i]), 14);
		this->m_ptr_object_fake_vtable[i] = (uintptr_t)((char*)this->m_ptr_proc_fake_table + (16 * i));
	}
	*reinterpret_cast<uintptr_t**>(object) = this->m_ptr_object_fake_vtable;
}

size_t VMTHook::get_function_count()
{
	MEMORY_BASIC_INFORMATION mbi{ };
	size_t i{ };
	while (VirtualQuery(reinterpret_cast<LPCVOID>(this->m_ptr_object_vtable[i]), &mbi, sizeof(mbi)))
	{
#define PAGE_EXECUTABLE ( PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY )
		if ((mbi.State != MEM_COMMIT) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(mbi.Protect & PAGE_EXECUTABLE))
			break;
		++i;
	}
	return i;
}
uintptr_t* VMTHook::Apply(int index, uintptr_t* hook_function)
{
	memcpy((char*)this->m_ptr_proc_fake_table + (16 * index), CodeJmpTo(hook_function), 14);
	this->m_object_hooks[index] = (uintptr_t*)this->m_ptr_object_vtable[index];
	return this->m_object_hooks[index];
}
uintptr_t* VMTHook::AddressAtIndexOrg(int index)
{
	BYTE* ptrtmp = (BYTE*)this->AddressAtIndex(index);
	if (ptrtmp)
	{
		ptrtmp = (ptrtmp + 5) + (*(UINT32*)&ptrtmp[1]);
		return (uintptr_t*)ptrtmp;
	}
	return 0;
}
uintptr_t* VMTHook::AddressAtIndex(int index)
{
	uintptr_t* p_original_func = *reinterpret_cast<uintptr_t**>(this->m_ptr_object_fake_vtable + index);
	return p_original_func;
}

void VMTHook::Remove(int index)
{
	memcpy((char*)this->m_ptr_proc_fake_table + (16 * index), CodeJmpTo(this->m_object_hooks[index]), 14);
	this->m_object_hooks.erase(index);
}