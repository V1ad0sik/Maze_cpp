#pragma once

HWND hwnd;
HANDLE process;

DWORD client;
DWORD engine;
DWORD vstdlibModule;
DWORD vstdlibModule_size;

template <typename T>
T readMem(DWORD address) { T buffer; ReadProcessMemory(process, (LPVOID)address, &buffer, sizeof(buffer), 0); return buffer; }

template <typename T>
void writeMem(DWORD address, T value) { WriteProcessMemory(process, (LPVOID)address, &value, sizeof(value), 0); }


DWORD getModuleBaseAddress(const char* name, DWORD procId)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry))
        {
            do { if (!strcmp(modEntry.szModule, name)) { CloseHandle(hSnap); return (uintptr_t)modEntry.modBaseAddr; } } while (Module32Next(hSnap, &modEntry));
        }
    }
}

DWORD getModuleSize(const char* name, DWORD procId)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry))
        {
            do { if (!strcmp(modEntry.szModule, name)) {CloseHandle(hSnap); return (uintptr_t)modEntry.modBaseSize;} } while (Module32Next(hSnap, &modEntry));
        }
    }
}


void get_addres_point()
{
    DWORD procId;

    hwnd = FindWindowA(0, "Counter-Strike: Global Offensive");
    GetWindowThreadProcessId(hwnd, &procId);

    bool game_status = GetWindowThreadProcessId(hwnd, &procId);

    if (game_status)
    {
        client = getModuleBaseAddress("client.dll", procId);
        engine = getModuleBaseAddress("engine.dll", procId);

        vstdlibModule = getModuleBaseAddress("vstdlib.dll", procId);
        vstdlibModule_size = getModuleSize("vstdlib.dll", procId);

        process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    }

    else
    {
        MessageBox(0, "Не удалось найти процесс.", "Ошибка!", MB_OK);
        exit(0);
    }
}
