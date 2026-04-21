package com.ejemplo.cheatmod.modules.combat;

import com.ejemplo.cheatmod.core.Module;
import com.ejemplo.cheatmod.core.Setting;
import com.ejemplo.cheatmod.utils.RotationUtils;
import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.Entity;
import net.minecraft.item.BowItem;
import net.minecraft.util.Hand;

public class BowAimbotModule extends Module {
    public final Setting<Double> range = new Setting<>("Range", 40.0);

    public BowAimbotModule() {
        super("BowAimbot", Category.COMBAT);
        settings.add(range);
    }

    @Override
    public void onTick() {
        MinecraftClient client = MinecraftClient.getInstance();
        if (!(client.player.getMainHandStack().getItem() instanceof BowItem)) return;
        if (!client.options.useKey.isPressed()) return;

        Entity target = TargetUtils.getBestTarget(range.getValue());
        if (target != null) {
            float[] rotations = RotationUtils.getRotations(target);
            // Simple prediction could be added here
            client.player.setYaw(rotations[0]);
            client.player.setPitch(rotations[1]);
        }
    }

    @Override public void onEnable() {}
    @Override public void onDisable() {}
}
