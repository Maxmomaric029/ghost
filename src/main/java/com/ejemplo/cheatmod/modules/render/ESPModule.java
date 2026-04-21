package com.ejemplo.cheatmod.modules.render;

import com.ejemplo.cheatmod.core.Module;
import com.ejemplo.cheatmod.core.Setting;
import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.Entity;
import net.minecraft.entity.player.PlayerEntity;
import net.minecraft.entity.mob.HostileEntity;

public class ESPModule extends Module {
    public final Setting<Boolean> players = new Setting<>("Players", true);
    public final Setting<Boolean> mobs = new Setting<>("Mobs", false);
    public final Setting<Double> range = new Setting<>("Range", 64.0);

    public ESPModule() {
        super("ESP", Category.RENDER);
        settings.add(players);
        settings.add(mobs);
        settings.add(range);
    }

    @Override public void onEnable() {}
    @Override public void onDisable() {}
}
