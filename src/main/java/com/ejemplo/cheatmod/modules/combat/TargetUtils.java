package com.ejemplo.cheatmod.modules.combat;

import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.Entity;
import net.minecraft.entity.LivingEntity;
import net.minecraft.entity.player.PlayerEntity;
import java.util.Comparator;
import java.util.stream.StreamSupport;

public class TargetUtils {
    public static Entity getBestTarget(double range) {
        MinecraftClient client = MinecraftClient.getInstance();
        if (client.world == null || client.player == null) return null;

        return StreamSupport.stream(client.world.getEntities().spliterator(), false)
            .filter(e -> e instanceof LivingEntity)
            .filter(e -> e != client.player)
            .filter(e -> !e.isRemoved())
            .filter(e -> ((LivingEntity) e).getHealth() > 0)
            .filter(e -> e.distanceTo(client.player) <= range)
            .min(Comparator.comparingDouble(e -> e.distanceTo(client.player)))
            .orElse(null);
    }
}
