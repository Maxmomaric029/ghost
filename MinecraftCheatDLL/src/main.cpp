#include <windows.h>
#include <iostream>
#include "CheatCore.h"

DWORD WINAPI CheatThread(LPVOID lpParam) {
    HMODULE hMod = (HMODULE)lpParam;
    
    while (!GetModuleHandleW(L"javaw.exe")) {
        Sleep(500);
    }

    __try {
        if (CheatCore::Instance().Initialize()) {
            while (CheatCore::Instance().IsActive()) {
                CheatCore::Instance().RunFrame();
                Sleep(16);
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        // Log crash or handle
    }

    CheatCore::Instance().Shutdown();
    FreeLibraryAndExitThread(hMod, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, CheatThread, hModule, 0, NULL);
    }
    return TRUE;
}
