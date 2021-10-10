#pragma once

#define NT_SUCCESS(x) ((x) >= 0)
typedef NTSTATUS(NTAPI* _NtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, LPCVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
_NtWriteVirtualMemory NtWriteVirtualMemory = (_NtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");
typedef NTSTATUS(NTAPI* _NtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
_NtReadVirtualMemory NtReadVirtualMemory = (_NtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");

template <class dataType>

void ReadVM(DWORD addressToRead, dataType* valToRead)
{
	DWORD oldProtect = 0; NTSTATUS status = 0;
	VirtualProtectEx(process, (PVOID)addressToRead, sizeof(dataType), PAGE_EXECUTE_READWRITE, &oldProtect);
	status = NtReadVirtualMemory(process, PVOID(addressToRead), valToRead, sizeof(dataType), 0);
	VirtualProtectEx(process, (PVOID)addressToRead, sizeof(dataType), oldProtect, NULL);
}

bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++bData, ++bMask)
	{
		if (*szMask == 'x' && *bData != *bMask)
		{
			return false;
		}
	}
	return (*szMask == NULL);
}

DWORD FindSignature(DWORD start, DWORD size, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[size];
	SIZE_T bytesRead;

	ReadProcessMemory(process, (LPVOID)start, data, size, &bytesRead);

	for (DWORD i = 0; i < size; i++)
	{
		if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask))
		{
			delete[] data;
			return start + i;
		}
	}
	delete[] data;
	return NULL;
}

int GetStringHash(const char* name)
{
	int codes[256];
	DWORD convarNameHashTable = readMem<DWORD>(FindSignature(vstdlibModule, vstdlibModule_size, "\x8B\x3C\x85", "xxx") + 3) - vstdlibModule;
	ReadVM(vstdlibModule + convarNameHashTable, &codes);

	int v2 = 0;
	int v3 = 0;

	for (int i = 0; i < strlen(name); i += 2)
	{
		v3 = codes[v2 ^ toupper(name[i])];
		if (i + 1 == strlen(name))
			break;
		v2 = codes[v3 ^ toupper(name[i + 1])];
	}

	return v2 | (v3 << 8);
}

DWORD GetConVarAddres(const char* name)
{
	int hash = GetStringHash(name);
	DWORD interfaceEngineCvar = readMem<DWORD>(FindSignature(vstdlibModule, vstdlibModule_size, "\x8B\x0D\x00\x00\x00\x00\xC7\x05", "xx????xx") + 0x2) - vstdlibModule;
	DWORD cvarEngine = readMem<DWORD>(vstdlibModule + interfaceEngineCvar);
	DWORD Pointer = readMem<DWORD>(readMem<DWORD>(cvarEngine + 0x34) + ((BYTE)hash * 4));

	while (Pointer)
	{
		if (readMem<DWORD>(Pointer) == hash)
		{
			DWORD ConVarPointer = readMem<DWORD>(Pointer + 0x4);
			char* varname[32];
			ReadVM(readMem<DWORD>(ConVarPointer + 0xC), &varname);
			if (!strcmp((char*)varname, name))
			{
				return ConVarPointer;
			}
		}

		Pointer = readMem<DWORD>(Pointer + 0xC);
	}
}

void SetValueFLOAT(const char* name, float value)
{
	DWORD address = GetConVarAddres(name);
	writeMem<int>(address + 0x2C, *(int*)&value ^ address);
}

void SetValueINT(const char* name, int value)
{
	DWORD address = GetConVarAddres(name);
	writeMem<int>(address + 0x30, value ^ address);
}
