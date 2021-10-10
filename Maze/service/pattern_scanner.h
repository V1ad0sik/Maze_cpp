#pragma once

HANDLE process_handle;

MODULEENTRY32 get_module(const char* modName, DWORD proc_id)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry))
        {
            do { if (!strcmp(modEntry.szModule, modName)) { CloseHandle(hSnap); return modEntry; } } while (Module32Next(hSnap, &modEntry));
        }
    }

    MODULEENTRY32 module = { -1 };
    return module;
}

uintptr_t get_pattern(MODULEENTRY32 module, uint8_t* arr, const char* pattern, int offset, int extra)
{
    uintptr_t scan = 0x0;
    const char* pat = pattern;
    uintptr_t firstMatch = 0;

    for (uintptr_t pCur = (uintptr_t)arr; pCur < (uintptr_t)arr + module.modBaseSize; ++pCur)
    {
        if (!*pat) { scan = firstMatch; break; }

        if (*(uint8_t*)pat == '\?' || *(uint8_t*)pCur == ((((pat[0] & (~0x20)) >= 'A' && (pat[0] & (~0x20)) <= 'F') ? ((pat[0] & (~0x20)) - 'A' + 0xa) : ((pat[0] >= '0' && pat[0] <= '9') ? pat[0] - '0' : 0)) << 4 | (((pat[1] & (~0x20)) >= 'A' && (pat[1] & (~0x20)) <= 'F') ? ((pat[1] & (~0x20)) - 'A' + 0xa) : ((pat[1] >= '0' && pat[1] <= '9') ? pat[1] - '0' : 0)))) {
            if (!firstMatch) firstMatch = pCur;
            if (!pat[2]) { scan = firstMatch; break; }
            if (*(WORD*)pat == 16191 || *(uint8_t*)pat != '\?') pat += 3;
            else pat += 2;
        }

        else { pat = pattern; firstMatch = 0; }
    }

    if (!scan) return 0x0;
    uint32_t read;
    ReadProcessMemory(process_handle, (void*)(scan - (uintptr_t)arr + (uintptr_t)module.modBaseAddr + offset), &read, sizeof(read), NULL);

    return read + extra;
}


void pattern_start()
{
    HWND hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
    DWORD proc_id; GetWindowThreadProcessId(hwnd, &proc_id);
    process_handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, proc_id);

    MODULEENTRY32 client = get_module("client.dll", proc_id);

    auto bytes = new uint8_t[client.modBaseSize];

    DWORD bytes_read;

    ReadProcessMemory(process_handle, client.modBaseAddr, bytes, client.modBaseSize, &bytes_read);
    if (bytes_read != client.modBaseSize) throw;

    // OFFSET && EXTRA.

    dwLocalPlayer = get_pattern(client, bytes, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 0x3, 0x4) - (uintptr_t)client.modBaseAddr;
    dwGlowObjectManager = get_pattern(client, bytes, "A1 ? ? ? ? A8 01 75 4B", 0x1, 0x4) - (uintptr_t)client.modBaseAddr;
    dwEntityList = get_pattern(client, bytes, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1, 0x0) - (uintptr_t)client.modBaseAddr;
    dwForceJump = get_pattern(client, bytes, "8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02", 0x2, 0x0) - (uintptr_t)client.modBaseAddr;
    dwForceAttack = get_pattern(client, bytes, "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04", 0x2, 0x0) - (uintptr_t)client.modBaseAddr;
    dwMouseEnable = get_pattern(client, bytes, "B9 ? ? ? ? FF 50 34 85 C0 75 10", 0x1, 0x30) - (uintptr_t)client.modBaseAddr;
}
