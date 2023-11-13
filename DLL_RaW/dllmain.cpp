#include <Windows.h>
#include <iostream>
#include <vector>
#include "memory.h"

DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD> offsets)
{
    DWORD addr = ptr;
    for (int i = 0; i < offsets.size(); ++i)
    {
        addr = *(DWORD*)addr;
        addr += offsets[i];
    }
    return addr;
}

DWORD WINAPI MainThread(HMODULE hmodule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    printf("Ass cube ammo hack B)");

    Sleep(2000);
    FreeConsole();

    DWORD moduelBase = (DWORD)GetModuleHandle("ac_client.exe");

    while (!GetAsyncKeyState(VK_END))
    {  
        int* ammo = (int*)GetPointerAddress(moduelBase + meme::Ammo, meme::ammo_offsets);
        *ammo = 69420;
    }

    if (GetAsyncKeyState(VK_END))
    {
        fclose(f);
    }

    FreeLibraryAndExitThread(hmodule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
