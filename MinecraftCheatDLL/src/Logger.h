#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdarg>
#include <iostream>

enum LogLevel { LOG_INFO, LOG_WARN, LOG_ERROR };

inline void Log(LogLevel level, const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    
    const char* prefix[] = { "[INFO]", "[WARN]", "[ERROR]" };
    std::string msg = std::string(prefix[level]) + " " + buf + "\n";
    
    // Imprimir en la consola asignada (AllocConsole)
    printf("%s", msg.c_str());

    OutputDebugStringA(msg.c_str());
    
    // También escribir a archivo para debug
    static std::ofstream logFile("ghost_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << msg;
        logFile.flush();
    }
}
