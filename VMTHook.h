#pragma once
#include <map>
#include <utility>

class VMTHook
{
private:
	void* m_ptr_object;
	uintptr_t* m_ptr_object_vtable;
	uintptr_t* m_ptr_object_fake_vtable;
	void* m_ptr_proc_fake_table;
	size_t					  m_object_vtable_size;
	std::map<int, uintptr_t*> m_object_hooks;

public:
	size_t get_function_count();
	uintptr_t* AddressAtIndex(int index);
	uintptr_t* AddressAtIndexOrg(int index);
	VMTHook(void* object);
	uintptr_t* Apply(int index, uintptr_t* hook_function);
	void Remove(int index);
};