#include <windows.h>
#include "CheatCore.h"
#include "JVMHelper.h"

DWORD WINAPI CheatThread(LPVOID lpParam) {
    HMODULE hMod = (HMODULE)lpParam;
    
    while (!GetModuleHandleW(L"javaw.exe")) {
        Sleep(500);
    }

    __try {
        if (JVMHelper::Initialize()) {
            JNIEnv* env = JVMHelper::AttachThreadToJVM();
            if (env) {
                HWND mcWnd = FindWindowW(L"GLFW30", L"Minecraft 1.21.4");
                if (mcWnd && CheatCore::Instance().Initialize(env, mcWnd)) {
                    while (CheatCore::Instance().IsRunning()) {
                        CheatCore::Instance().RunFrame();
                        Sleep(16);
                    }
                }
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        OutputDebugStringA("[GhostClient] Excepción fatal en el hilo principal.");
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
