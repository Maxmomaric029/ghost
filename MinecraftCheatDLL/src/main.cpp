#include <windows.h>
#include "CheatCore.h"
#include "JVMHelper.h"

// Estructura para búsqueda robusta de ventana
struct WindowFinder {
    HWND foundHandle;
    const wchar_t* titlePattern;
};

// Callback para EnumWindows: Busca por clase GLFW30 y contenido de título
BOOL CALLBACK FindMinecraftWindow(HWND hwnd, LPARAM lParam) {
    WindowFinder* finder = (WindowFinder*)lParam;
    
    wchar_t className[256];
    GetClassNameW(hwnd, className, 256);
    
    if (wcscmp(className, L"GLFW30") == 0) {
        wchar_t title[256];
        GetWindowTextW(hwnd, title, 256);
        
        if (wcsstr(title, finder->titlePattern) != nullptr) {
            finder->foundHandle = hwnd;
            return FALSE; // Detener búsqueda al encontrarla
        }
    }
    return TRUE;
}

/**
 * Hilo principal del cheat. Gestiona el ciclo de vida y la inyección.
 */
DWORD WINAPI CheatMainThread(LPVOID lpParam) {
    HMODULE hModule = (HMODULE)lpParam;
    WindowFinder finder = { NULL, L"Minecraft" };

    OutputDebugStringA("[GhostClient] Hilo principal iniciado.");

    // Búsqueda robusta de la ventana del juego
    while (finder.foundHandle == NULL) {
        EnumWindows(FindMinecraftWindow, (LPARAM)&finder);
        if (finder.foundHandle == NULL) Sleep(1000);
    }

    OutputDebugStringA("[GhostClient] Ventana de Minecraft detectada.");

    // Inicialización del motor JNI y Core
    if (JVMHelper::Initialize()) {
        JavaVM* jvm = JVMHelper::GetJVM();
        JNIEnv* env;
        
        if (jvm->AttachCurrentThread((void**)&env, nullptr) == JNI_OK) {
            if (CheatCore::Instance().Initialize(finder.foundHandle)) {
                OutputDebugStringA("[GhostClient] Inicialización completada con éxito.");
                
                // Bucle principal de ejecución (60 FPS aprox)
                while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
                    CheatCore::Instance().Run();
                    Sleep(16);
                }
            }
            
            // Limpieza al salir
            OutputDebugStringA("[GhostClient] Cerrando cheat y liberando recursos.");
            CheatCore::Instance().Shutdown();
            JVMHelper::Cleanup();
            
            // Desvincular antes de morir para evitar crashes
            jvm->DetachCurrentThread();
        }
    }

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

/**
 * Punto de entrada del DLL. Crea el hilo de ejecución para no bloquear el proceso.
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, CheatMainThread, hModule, 0, NULL);
        if (hThread) CloseHandle(hThread);
        break;
    }
    return TRUE;
}
