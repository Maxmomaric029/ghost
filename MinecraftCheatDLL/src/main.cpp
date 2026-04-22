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
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    SetConsoleTitleA("Antigravity Ghost - Debug Console");
    printf("[+] Consola activa\n");

    HMODULE hModule = (HMODULE)lpParam;
    WindowFinder finder = { NULL, L"Minecraft" };

    printf("[*] Buscando ventana de Minecraft...\n");
    while (finder.foundHandle == NULL) {
        EnumWindows(FindMinecraftWindow, (LPARAM)&finder);
        if (finder.foundHandle == NULL) {
            printf("[*] No encontrada, reintentando...\n");
            Sleep(1000);
        }
    }
    printf("[+] Ventana encontrada: %p\n", finder.foundHandle);

    printf("[*] Inicializando JVM...\n");
    if (!JVMHelper::Initialize()) {
        printf("[!] JVM FALLO - No se encontro la Virtual Machine activa.\n");
        Sleep(5000);
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        return 0;
    }
    printf("[+] JVM OK\n");

    printf("[*] Inicializando CheatCore...\n");
    if (CheatCore::Instance().Initialize(finder.foundHandle)) {
        printf("[+] CheatCore OK - Ejecutando\n");
        
        while (!((GetKeyState(VK_END) & 0x8000) != 0)) {
            CheatCore::Instance().Run();
            Sleep(16);
        }
    } else {
        printf("[!] CheatCore FALLO - Revisa los logs arriba para ver que clase u offset fallo.\n");
        Sleep(10000);
    }

    printf("[*] Cerrando cheat y liberando recursos...\n");
    CheatCore::Instance().Shutdown();
    JVMHelper::Cleanup();
    
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

/**
 * Punto de entrada del DLL. Crea el hilo de ejecución para no bloquear el proceso.
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, CheatMainThread, hModule, 0, NULL);
        if (hThread) CloseHandle(hThread);
        break;
    }
    }
    return TRUE;
}
