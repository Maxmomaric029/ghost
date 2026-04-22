#pragma once
#include <fstream>
#include "json.hpp"
#include <windows.h>

using json = nlohmann::json;

struct Config {
    bool aimbot = true;
    bool esp = true;
    float aimbotFov = 45.0f;
    float aimbotSmooth = 0.15f;
    float aimbotRange = 6.0f;
    
    void Save() {
        try {
            json j = { 
                {"aimbot", aimbot}, 
                {"esp", esp}, 
                {"fov", aimbotFov}, 
                {"smooth", aimbotSmooth},
                {"range", aimbotRange}
            };
            std::ofstream f("ghost_config.json");
            if (f.is_open()) {
                f << j.dump(4);
            }
        } catch (...) {}
    }

    void Load() {
        try {
            std::ifstream f("ghost_config.json");
            if (!f.is_open()) return;
            json j = json::parse(f);
            aimbot = j.value("aimbot", true);
            esp = j.value("esp", true);
            aimbotFov = j.value("fov", 45.0f);
            aimbotSmooth = j.value("smooth", 0.15f);
            aimbotRange = j.value("range", 6.0f);
        } catch (...) {}
    }
};
