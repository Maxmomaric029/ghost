package com.ejemplo.cheatmod.utils;

import net.minecraft.util.math.MathHelper;

public class MathUtils {
    public static double lerp(double delta, double start, double end) {
        return start + delta * (end - start);
    }

    public static float clampPitch(float pitch) {
        return MathHelper.clamp(pitch, -90.0f, 90.0f);
    }
}
