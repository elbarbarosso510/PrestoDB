#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <WinInet.h>
#include <sstream>
#include <string>
#include <urlmon.h>
#include <Psapi.h>
#include <cstdint>
#include <codecvt>
#include <intrin.h>
#include "List.h"
#include "Tuple.h"
#include <d3d11.h>
#include <unordered_map>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "Wininet.lib") 
#define DBG_PRINT(x,...) printf_s("%s[LINE %d]:%s\n",__FILE__,__LINE__,FormatA(x,__VA_ARGS__))
#define DBG_PRINT_LINE DBG_PRINT("EXEC")
using namespace std;
#ifndef typeof
#define typeof(x) decltype(x)
#endif // !typeof

#ifndef PROPERTY
#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n)) t n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n
#define GET(t,n) t property__get_##n() 
#define SET(t,n) void property__set_##n(const t& value)
#endif // !PROPERTY

#ifndef ENCRYPT_SYSTEM_TYPE
#define ENCRYPT_SYSTEM_TYPE(type,name)\
unsigned long long _##name;\
PROPERTY(type, name);\
GET(type, name)\
{\
	unsigned long long tmpval = _##name ^ ((unsigned long long)&_##name);\
	type result;\
	memcpy(&result, &tmpval, sizeof(result));\
	return result;\
}\
SET(type, name)\
{\
	unsigned long long tmpval = 0;\
	memcpy(&tmpval, &value, sizeof(value));\
	_##name = tmpval ^ ((unsigned long long)&_##name);\
}
#endif // !ENCRYPT_SYSTEM_TYPE

#define MAP_DBG_ADDRESS 0x4A00000000
#define EXEC_LINE_VAL (*(ULONG64*)MAP_DBG_ADDRESS)
#define EXEC_LINE EXEC_LINE_VAL=__LINE__
#ifndef _IMAGE_DEBUG_DIRECTORY_RAW_
#define _IMAGE_DEBUG_DIRECTORY_RAW_
typedef struct _IMAGE_DEBUG_DIRECTORY_RAW {
	uint8_t format[4];
	uint8_t PdbSignature[16];
	uint32_t PdbDbiAge;
	uint8_t ImageName[256];
	const char* GuidString()
	{
		char buf[128];
		sprintf_s(buf, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
			this->PdbSignature[3], this->PdbSignature[2],
			this->PdbSignature[1], this->PdbSignature[0],
			this->PdbSignature[5], this->PdbSignature[4],
			this->PdbSignature[7], this->PdbSignature[6],
			this->PdbSignature[8], this->PdbSignature[9],
			this->PdbSignature[10], this->PdbSignature[11],
			this->PdbSignature[12], this->PdbSignature[13],
			this->PdbSignature[14], this->PdbSignature[15]
		);
		return buf;
	}
} IMAGE_DEBUG_DIRECTORY_RAW, * PIMAGE_DEBUG_DIRECTORY_RAW;
#endif

#ifdef _WIN64
#define XIP Rip
#else
#define XIP Eip
#endif
class RegisterCapture
{
	static void* VEH_Handle;
	static void* ptr;
	static BYTE old_char;
	static bool Captured;
	static DWORD oldProtection;
	static void Restore()
	{
		*(BYTE*)ptr = old_char;
		DWORD tmpp = 0;
		VirtualProtect((LPVOID)ptr, 0x1000, oldProtection, &tmpp);
		RemoveVectoredExceptionHandler(VEH_Handle);
	}
	static LONG WINAPI ExHandler(EXCEPTION_POINTERS* pExceptionInfo)
	{
		if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
		{
			if (pExceptionInfo->ContextRecord->XIP == (ULONG64)ptr)
			{
				memcpy(&ContextLog, pExceptionInfo->ContextRecord, sizeof(ContextLog));
				Restore();
				Captured = true;
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
public:
	static CONTEXT ContextLog;
	static bool Capture(void* addr, int ms = 0)
	{
		ZeroMemory(&ContextLog, sizeof(ContextLog));
		Captured = false;
		RegisterCapture::ptr = addr;
		VEH_Handle = AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)RegisterCapture::ExHandler);
		if (VEH_Handle && VirtualProtect((LPVOID)ptr, 0x1000, PAGE_EXECUTE_READWRITE, &oldProtection))
		{
			old_char = *(BYTE*)ptr;
			*(BYTE*)ptr = 0xCC;
			ULONG64  ems = GetTickCount64() + ms;
			while (Captured == false)
			{
				if (GetTickCount64() > ems && ms > 0)
				{
					Restore();
					return false;
				}
				Sleep(1);
			}
			return true;
		}
		return false;
	}
};
bool RegisterCapture::Captured = false;
BYTE RegisterCapture::old_char = 0;
void* RegisterCapture::ptr = 0;
void* RegisterCapture::VEH_Handle = nullptr;
DWORD RegisterCapture::oldProtection = 0;
CONTEXT RegisterCapture::ContextLog = { 0 };
#pragma region TPattern
class TPattern
{
	std::string sMask;
	std::string sPattern;
	std::vector<uint8_t> bytes;
	bool bIdaStyle = false;

private:
	void Parse()
	{
		if (bIdaStyle)
		{
			ParsePatternIDA();
		}
		else
		{
			ParsePattern();
		}
	}
	void ParsePattern()
	{
		auto uLen = sMask.length();
		for (uint32_t i = 0; i < uLen; i++)
		{
			if (i < uLen && sPattern[i] == 0)
			{
				sPattern[i] = (char)0xFF;
			}
			if (sMask[i] == '?')
			{
				bytes.push_back(0);
			}
			else
			{
				auto sub = sPattern.substr(i, 1);
				auto b = uint8_t(sub[0]);
				bytes.push_back(b);
			}
		}
	}

	void ParsePatternIDA()
	{
		auto uLen = sPattern.length();
		sMask.clear();
		int b = 0;
		char x, byteBuf = 0;
		for (uint32_t i = 0; i < uLen; i++)
		{
			char c = (char)sPattern[i];
			if (c >= '0' && c <= '9')
			{
				x = (0x0F & c);
				byteBuf += (b) ? x : x << 4;
				b++;
			}
			else if (c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f')
			{
				if (c > 0x46)
				{
					c -= 0x20;
				}
				x = c - 0x37;
				byteBuf += (b) ? x : x << 4;
				b++;
			}
			else if (c == '?')
			{
				if (sPattern[i + 1] == '?')
				{
					i++;
				}
				bytes.push_back(0);
				sMask += '?';
			}
			if (b > 1)
			{
				bytes.push_back(byteBuf);
				b = 0;
				byteBuf = 0;
				sMask += 'x';
			}
		}
	}
public:

	TPattern(const char* sPattern)
	{
		bIdaStyle = true;
		this->sPattern = sPattern;
		Parse();
	}

	TPattern(const char* sPattern, const char* sMask)
	{
		this->sMask = sMask;
		bytes.resize(this->sMask.length());
		memcpy(&bytes[0], sPattern, bytes.size());
		Parse();
	}

	std::vector<uint8_t> Get()
	{
		return bytes;
	}

	std::string GetMask()
	{
		return sMask;
	}

	size_t Size()
	{
		return bytes.size();
	}
};

static void* Scan(void* pAddress, size_t size, TPattern* pattern)
{
	auto pat = pattern->Get();
	auto mask = pattern->GetMask();
	uint8_t* pByte = (uint8_t*)pAddress;
	size_t j = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (IsBadReadPtr(pByte + i, pat.size()))
		{
			continue;
		}
		if (pByte[i] == pat[j] || mask[j] == '?')
		{
			j++;
			if (j == pat.size() - 1)
			{
				return (void*)((ULONG64)pByte + i - pat.size() + 2);
			}
		}
		else
		{
			j = 0;
		}
	}
	return nullptr;
}
static void* ScanPattern(const char* szModule, const char* sPattern)
{
	TPattern* pattern = new TPattern(sPattern);
	OutputDebugStringA(pattern->GetMask().c_str());
	MODULEINFO mi{ };
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &mi, sizeof(mi));
	LPVOID dwBaseAddress = mi.lpBaseOfDll;
	const auto dwModuleSize = mi.SizeOfImage;
	return Scan(dwBaseAddress, dwModuleSize - pattern->Size(), pattern);
}
#pragma endregion

static char* TCHAR2char(const wchar_t* STR)
{
	static const char* _EMPTY = "";
	if (STR)
	{
		int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);
		char* str = new char[sizeof(char) * size];
		WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);
		return str;
	}
	return (char*)_EMPTY;

}
static wchar_t* Char2TCHAR(const char* _char)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	wchar_t* tchar = new wchar_t[sizeof(wchar_t) * iLength];
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	return tchar;
}
static string wstring_to_string(wstring s)
{
	return string(TCHAR2char(s.c_str()));
}
static wstring string_to_wstring(string s)
{
	return wstring(Char2TCHAR(s.c_str()));
}
static char* ToHex(const char* data, size_t len)
{
	const char* key = "0123456789ABCDEF";
	char* result = new char[len * 2];
	for (int i = 0; i < len; i++)
	{
		result[i * 2] = key[(BYTE)(data[i] >> 4)];
		result[(i * 2) + 1] = key[(BYTE)((BYTE)(data[i] << 4) >> 4)];
	}
	return result;
}
static char* ToHex(const BYTE* data, size_t len)
{
	const char* key = "0123456789ABCDEF";
	char* result = new char[(len * 2) + 1];
	for (int i = 0; i < len; i++)
	{
		result[i * 2] = key[(BYTE)(data[i] >> 4)];
		result[(i * 2) + 1] = key[(BYTE)((BYTE)(data[i] << 4) >> 4)];
	}
	result[len * 2] = '\0';
	return result;
}
static vector<BYTE> ParseHex(const char* hex)
{
	vector<BYTE> result = vector<BYTE>();
	int len = strlen(hex);
	BYTE* buffer = new BYTE[len];
	bool ish = true;
	BYTE val = 0;
	for (int i = 0; i < len; i++)
	{
		bool isval = false;
		BYTE v = 0;
		if (hex[i] >= '0' && hex[i] <= '9')
		{
			v = (hex[i] - '0');
			isval = true;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F')
		{
			v = 10 + (hex[i] - 'A');
			isval = true;
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f')
		{
			v = 10 + (hex[i] - 'a');
			isval = true;
		}
		if (isval)
		{
			if (ish)
			{
				val = (v << 4) | val;
			}
			else
			{
				val = v | val;
				result.push_back(val);
				val = 0;
			}
			ish = !ish;
		}
	}
	return result;
}
static initializer_list<unsigned char>* ReadAllBytes(const char* filePath)
{
	HANDLE pFile;
	DWORD fileSize;
	unsigned char* buffer, * tmpBuf;
	DWORD dwBytesRead, dwBytesToRead, tmpLen;

	pFile = CreateFileA(filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		return nullptr;
	}
	fileSize = GetFileSize(pFile, NULL);
	buffer = (unsigned char*)malloc(fileSize);
	if (!buffer)
	{
		return nullptr;
	}
	ZeroMemory(buffer, fileSize);
	dwBytesToRead = fileSize;
	dwBytesRead = 0;
	tmpBuf = buffer;
	do {
		if (!ReadFile(pFile, tmpBuf, dwBytesToRead, &dwBytesRead, NULL) || dwBytesRead == 0)
			break;
		dwBytesToRead -= dwBytesRead;
		tmpBuf += dwBytesRead;

	} while (dwBytesToRead > 0);
	CloseHandle(pFile);
	return new initializer_list<unsigned char>(&buffer[0], &buffer[fileSize]);
}
static BOOL WriteAllBytes(const char* filePath, char* buffer, int len)
{
	HANDLE pFile;
	char* tmpBuf;
	DWORD dwBytesWrite, dwBytesToWrite;

	pFile = CreateFileA(filePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		return FALSE;
	}

	dwBytesToWrite = len;
	dwBytesWrite = 0;

	tmpBuf = buffer;

	do {

		WriteFile(pFile, tmpBuf, dwBytesToWrite, &dwBytesWrite, NULL);

		dwBytesToWrite -= dwBytesWrite;
		tmpBuf += dwBytesWrite;

	} while (dwBytesToWrite > 0);

	CloseHandle(pFile);

	return TRUE;
}
static BOOL WriteAllBytes(const char* filePath, BYTE* buffer, int len)
{
	return WriteAllBytes(filePath, (char*)buffer, len);
}
static BOOL WriteAllBytes(const char* filePath, vector<BYTE> buffer)
{
	return WriteAllBytes(filePath, (char*)buffer.data(), buffer.size());
}
static BOOL WriteAllBytes(const char* filePath, initializer_list<BYTE> buffer)
{
	return WriteAllBytes(filePath, (char*)buffer.begin(), buffer.size());
}
static BOOL WriteAllBytes(const char* filePath, initializer_list<char> buffer)
{
	return WriteAllBytes(filePath, (char*)buffer.begin(), buffer.size());
}
static BOOL WriteAllText(const char* filePath, const char* text)
{
	return WriteAllBytes(filePath, (char*)text, strlen(text));
}
static vector<BYTE> HttpRequest(const char* url)
{
#define MAXSIZE 4096
	vector<BYTE> result = vector<BYTE>();
	BYTE* val = new BYTE[MAXSIZE];
	ULONG len = 1;
	HINTERNET hSession = InternetOpenA(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrlA(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

		if (hHttp != NULL)
		{
			while (len > 0)
			{
				InternetReadFile(hHttp, val, MAXSIZE - 1, &len);
				if (len > 0)
				{
					int ldx = result.size();
					result.resize(result.size() + len);
					memcpy(&result.operator[](ldx), &val[0], len);
				}
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
	return result;
}

static string WebRequest(const char* url)
{
#define BLOCK_READ 4096
	IStream* stream;
	HRESULT result = URLOpenBlockingStreamA(0, url, &stream, 0, 0);
	if (result != 0)
	{
		return "";
	}
	char buffer[BLOCK_READ];
	unsigned long bytesRead;
	stringstream ss;
	stream->Read(buffer, BLOCK_READ, &bytesRead);
	while (bytesRead > 0U)
	{
		ss.write(buffer, (long long)bytesRead);
		stream->Read(buffer, BLOCK_READ, &bytesRead);
	}
	stream->Release();
	return ss.str();
}


static BYTE* _get_jmp_address(BYTE* ptr)
{
	if (*ptr == 0xE9)
	{
		return (ptr + 5) + (*(UINT32*)&ptr[1]);
	}
	return ptr;
}
static int _get_index(BYTE* ptr)
{
	if (*(UINT16*)ptr == 0x20FF)
	{
		return 0;
	}
	if (*(UINT16*)ptr == 0x60FF)
	{
		return ptr[2] / sizeof(void*);
	}
	if (*(UINT16*)ptr == 0xA0FF)
	{
		return *(UINT32*)&ptr[2] / sizeof(void*);
	}
	return -1;
}
template<typename T>
static int GetVTableIndex(T funcaddr)
{
	void* proc = (void*&)funcaddr;
	BYTE* addr = _get_jmp_address((BYTE*)proc);
	for (int i = 0; i < 32; i++)
	{
		if (addr[i] == 0xFF)
		{
			auto dx = _get_index(&addr[i]);
			if (dx >= 0) return dx;
		}
	}
	return -1;
}

static VOID read_memory_bytes(UINT64 ptr, SIZE_T size, PVOID buff)
{
	if (ptr < 0x1000000 - 0x8 || ptr > 0x7FFFFFF00000 + 0x8)
	{
		__stosb((PBYTE)buff, 0, size);
		return;
	}
	else {
		if (IsBadReadPtr((PVOID)ptr, size)) {
			__stosb((PBYTE)buff, 0, size);
			return;
		}
		else {
			__movsb((PBYTE)buff, (BYTE*)ptr, size);
			return;
		}
	}
	return;

}

static VOID write_memory_bytes(UINT64 ptr, SIZE_T size, PVOID buff)
{
	if (ptr < 0x1000000 - 0x8 || ptr > 0x7FFFFFF00000 + 0x8)
	{
		__stosb((PBYTE)buff, 0, size);
		return;
	}
	else
	{
		if (!IsBadReadPtr((PVOID)ptr, size))
		{
			DWORD lpflOldProtect = 0;
			VirtualProtect((PVOID)ptr, size, PAGE_EXECUTE_WRITECOPY, &lpflOldProtect);
			__movsb((BYTE*)ptr, (PBYTE)buff, size);
			VirtualProtect((PVOID)ptr, size, lpflOldProtect, &lpflOldProtect);
			return;
		}
	}
	return;
}

static __int64 SearchBytesA(unsigned char* a, __int64 lena, unsigned char* b, __int64 lenb, __int64 startp)
{
	if (startp == -1)
	{
		startp = 0;
	}
	for (__int64 i = startp; i < lena; i++)
	{
		for (__int64 x = 0; x < lenb; x++)
		{
			if (b[x] == 0xCC)
			{
				continue;
			}
			if (a[i + x] != b[x])
			{
				break;
			}

			if (x == lenb - 1)
			{
				return i;
			}
		}
	}
	return -1;
}

static __int64 FindAsmCode(unsigned char* code, int code_size, __int64 startbase, __int64 basesize)
{
	unsigned char* p = new unsigned char[0x1000];

	for (__int64 i = 0; i < basesize; i += 0x1000)
	{

		read_memory_bytes(startbase + i, 0x1000, p);

		__int64 index = SearchBytesA(p, 0x1000, code, code_size, 0);

		if (index != -1)
		{
			return  i + index;
		}
	}
	return -1;
}
static POINT GetMousePos(HWND Hwnd)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(Hwnd, &p);
	return p;
}
static bool InRect(POINT point, POINT LOC, SIZE SIZ)
{
	int xf = point.x - LOC.x;
	int yf = point.y - LOC.y;
	return  xf >= 0 && xf <= SIZ.cx &&
		yf >= 0 && yf <= SIZ.cy;
}

static int maxof_list(List<ULONG64>& lst)
{
	ULONG64 maxv = 0;
	int res = -1;
	for (int i = 0; i < lst.Count; i++)
	{
		if (lst[i] < maxv)
		{
			maxv = lst[i];
			res = i;
		}
	}
	if (res >= 0)
	{
		ULONG64 tmp = lst[res];
		lst[res] = lst[0];
		lst[0] = tmp;
	}
	return res;
}
static DWORD NtOSBuildVersion()
{
	typedef LONG(NTAPI* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
	fnRtlGetVersion pRtlGetVersion = NULL;

	while (pRtlGetVersion == 0)
	{
		HMODULE ntdll = GetModuleHandleA("ntdll.dll");
		if (ntdll)
		{
			pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(ntdll, "RtlGetVersion");
		}
	}

	RTL_OSVERSIONINFOW osversion{};
	pRtlGetVersion(&osversion);
	return osversion.dwBuildNumber;
}
static struct IDXGISwapChain* FindDwmIDXGISwapChain()
{
	struct IDXGISwapChain* swap = NULL;
	DWORD nt = NtOSBuildVersion();
	if (nt >= 19041)
	{
		auto ptr = (ULONG64)ScanPattern("dwmcore.dll",
			"57 48 83 EC 60 8B 99 ? 04 00 00 ? 8B ? 48 8B");
		auto ptr1 = (ULONG64)ScanPattern("dwmcore.dll",
			"57 48 83 EC 50 8B 99 ? 04 00 00 ? 8B ? 48 8B");
		if (ptr && RegisterCapture::Capture((void*)(ptr + 1), 1000))
		{
			swap = (struct IDXGISwapChain*)RegisterCapture::ContextLog.Rdx;
		}
		else if (ptr1 && RegisterCapture::Capture((void*)(ptr1 + 1), 1000))
		{
			swap = (struct IDXGISwapChain*)RegisterCapture::ContextLog.Rdx;
		}
	}
	else if (nt >= 9200)
	{
		printf("FINDING...\n");
		auto t1 = (ULONG64)ScanPattern("dwmcore.dll",
			"01 00 00 8A 84 24 ? ? ? ?");
		if (t1)
		{
			ULONG64 CDWMSwapChain_VTable = NULL;
			BYTE* tmp = (BYTE*)t1;
			for (int i = 0; i < 100; i++)
			{
				tmp--;
				if ((tmp[0] == 0x48 && tmp[1] == 0x8D && tmp[2] == 0x0D) ||
					(tmp[0] == 0x4C && tmp[1] == 0x8D && tmp[2] == 0x1D))
				{
					CDWMSwapChain_VTable = *(int*)(tmp + 3) + ((ULONG64)tmp + 7);
					printf("CDWMSwapChain_VTable=0x%p\n", CDWMSwapChain_VTable);
					break;
				}
			}
			if (CDWMSwapChain_VTable)
			{
				ULONG64* table = (ULONG64*)CDWMSwapChain_VTable;
				for (int i = 0; i < 32; i++)
				{
					BYTE* code = (BYTE*)table[i];
					if (code[0] == 0x8A && code[1] == 0x81 && code[4] == 0x00 && code[5] == 0x00 && code[6] == 0xC3)
					{
						void* SupportsMoveOptimization = code;
						printf("SupportsMoveOptimization=0x%p\n", SupportsMoveOptimization);
						if (RegisterCapture::Capture(SupportsMoveOptimization, 1000))
						{
							auto SwapBase = RegisterCapture::ContextLog.Rcx;
							printf("SwapBase=0x%p\n", SwapBase);
							int Offset = 0x35;
							if (nt <= 7601) Offset = 0x17;
							else if (nt <= 9200) Offset = 0x24;
							else if (nt <= 14393) Offset = 0x25;
							else if (nt <= 15063) Offset = 0x26;
							else if (nt <= 16299) Offset = 0x34;
							else if (nt <= 18363) Offset = 0x35;
							swap = (struct IDXGISwapChain*)*((ULONG64*)SwapBase + Offset);
							printf("IDXGISwapChain=0x%p\n", swap);
							break;
						}
					}
				}
			}
		}
	}
	else if (nt == 7600 || nt == 7601)
	{
		auto ptr = (ULONG64)ScanPattern("dwmcore.dll",
			"33 ED 48 8B FA 3B DD");
		if (RegisterCapture::Capture((void*)ptr, 1000))
		{
			auto SwapBase = RegisterCapture::ContextLog.Rdx;
			swap = (struct IDXGISwapChain*)*((ULONG64*)SwapBase + 0x17);
		}
	}
	return swap;
}
static ULONG64 FindSwapChainPresentAddress()
{
	auto CDXGISwapChain_ValidatePresent = (ULONG64)ScanPattern("dxgi.dll",
		"56 "
		"57 "
		"41 54 "
		"41 55 "
		"41 56 "
		"41 57 "
		"48 8D 6C 24 F9 "
		"48 81 EC E0 00 00 00 "
		"48 C7 45 ? FE FF FF FF "
		"48 89 9C 24 ? 01 00 00 "
		"48 8B 05 ? ? ? ?");
	if (CDXGISwapChain_ValidatePresent)
	{
		printf("CDXGISwapChain_ValidatePresent=0x%X;\n", CDXGISwapChain_ValidatePresent);
		return (ULONG64)CDXGISwapChain_ValidatePresent - 2;
	}
	CDXGISwapChain_ValidatePresent = (ULONG64)ScanPattern("dxgi.dll",
		"55 "
		"56 "
		"57 "
		"41 54 "
		"41 55 "
		"41 56 "
		"41 57 "
		"48 8D 6C 24 F9 "
		"48 81 EC E0 00 00 00 "
		"48 8B 05 ? ? ? ? "
		"48 33 C4 "
		"48 89 ? ? "
		"4D 8B D1 ");
	if (CDXGISwapChain_ValidatePresent)
	{
		printf("CDXGISwapChain_ValidatePresent=0x%X;\n", CDXGISwapChain_ValidatePresent);
		return (ULONG64)CDXGISwapChain_ValidatePresent - 5;
	}
	return NULL;
}
static wchar_t* CurrentProcessName()
{
	TCHAR Buffer[MAX_PATH];
	GetModuleFileName(NULL, Buffer, MAX_PATH);
	return Buffer;
}
template <typename... Types>
static wchar_t* FormatW(const wchar_t* fmt, Types... args)
{
	static wchar_t TMP_[128];
	swprintf_s(TMP_, fmt, args...);
	return TMP_;
}
template <typename... Types>
static wchar_t* FormatAllocW(const wchar_t* fmt, Types... args)
{
	wchar_t* TMP_ = new wchar_t[0x100];
	swprintf_s(TMP_, 0x100, fmt, args...);
	return TMP_;
}
template <typename... Types>
static char* FormatA(const char* fmt, Types... args)
{
	static char TMP_[128];
	sprintf_s(TMP_, fmt, args...);
	return TMP_;
}

static unsigned short utf8_to_utf16(char* val)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	std::u16string dest = convert.from_bytes(val);
	return *(unsigned short*)&dest[0];
}
static std::string string_to_utf8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}
static std::string utf8_to_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}
static HWND GetWindowInProcess(const char* title, ULONG pid)
{
	static char title_tmp[128];
	static ULONG pid_tmp = 0;
	static HWND result = NULL;
	pid_tmp = pid;
	strcpy(title_tmp, title);
	EnumWindows([](HWND hwnd, LPARAM lParam)->BOOL
		{
			char caption[200];
			memset(caption, 0, sizeof(caption));
			GetWindowTextA(hwnd, caption, 200);
			ULONG thispid = 0;
			GetWindowThreadProcessId(hwnd, &thispid);
			if (strcmp(caption, title_tmp) == 0 && thispid == pid_tmp)
			{
				result = hwnd;
			}
			return TRUE;
		}, NULL);
	return result;
}