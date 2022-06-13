#include <Windows.h>
#include <iostream>
#include "Hacks/Hooks/Hooks.h"



DWORD WINAPI InitCheat(HMODULE hModule)
{
    Hooks::Attach();
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);


    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(50);
    }
    
    Hooks::Detach();

    fclose(f);
    FreeConsole();


    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitCheat, hModule, 0, nullptr);
    }
    return TRUE;
}

