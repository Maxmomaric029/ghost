package com.ejemplo.cheatmod.modules.combat;

import com.ejemplo.cheatmod.core.Module;
import com.ejemplo.cheatmod.core.Setting;
import com.ejemplo.cheatmod.utils.RotationUtils;
import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.Entity;
import net.minecraft.util.Hand;

public class KillAuraModule extends Module {
    public final Setting<Double> range = new Setting<>("Range", 3.8);
    public final Setting<Boolean> autoAttack = new Setting<>("AutoAttack", true);

    public KillAuraModule() {
        super("KillAura", Category.COMBAT);
        settings.add(range);
        settings.add(autoAttack);
    }

    @Override
    public void onTick() {
        MinecraftClient client = MinecraftClient.getInstance();
        Entity target = TargetUtils.getBestTarget(range.getValue());

        if (target != null) {
            float[] rotations = RotationUtils.getRotations(target);
            client.player.setYaw(rotations[0]);
            client.player.setPitch(rotations[1]);

            if (autoAttack.getValue() && client.player.getAttackCooldownProgress(0) >= 1.0f) {
                client.interactionManager.attackEntity(client.player, target);
                client.player.swingHand(Hand.MAIN_HAND);
            }
        }
    }

    @Override public void onEnable() {}
    @Override public void onDisable() {}
}
