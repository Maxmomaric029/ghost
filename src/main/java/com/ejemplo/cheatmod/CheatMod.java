package com.ejemplo.cheatmod;

import com.ejemplo.cheatmod.core.ModuleManager;
import com.ejemplo.cheatmod.event.EventManager;
import net.fabricmc.api.ClientModInitializer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CheatMod implements ClientModInitializer {
    public static final String MOD_ID = "cheatmod";
    public static final Logger LOGGER = LoggerFactory.getLogger(MOD_ID);
    public static final EventManager EVENT_MANAGER = new EventManager();
    public static ModuleManager MODULE_MANAGER;

    @Override
    public void onInitializeClient() {
        LOGGER.info("Inicializando CheatMod para 1.21.4...");
        MODULE_MANAGER = new ModuleManager();
    }
}
