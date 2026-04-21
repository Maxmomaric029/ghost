package com.ejemplo.cheatmod.core;

import com.ejemplo.cheatmod.modules.combat.KillAuraModule;
import com.ejemplo.cheatmod.modules.combat.BowAimbotModule;
import com.ejemplo.cheatmod.modules.render.ESPModule;
import java.util.ArrayList;
import java.util.List;

public class ModuleManager {
    private final List<Module> modules = new ArrayList<>();

    public ModuleManager() {
        modules.add(new ESPModule());
        modules.add(new KillAuraModule());
        modules.add(new BowAimbotModule());
    }

    public List<Module> getModules() {
        return modules;
    }

    public Module getModuleByName(String name) {
        return modules.stream().filter(m -> m.getName().equalsIgnoreCase(name)).findFirst().orElse(null);
    }

    public void onTick() {
        for (Module m : modules) {
            if (m.isEnabled()) {
                m.onTick();
            }
        }
    }
}
