package com.ejemplo.cheatmod.utils;

import net.minecraft.client.MinecraftClient;
import net.minecraft.client.render.Camera;
import net.minecraft.util.math.Vec3d;
import org.joml.Matrix4f;
import org.joml.Vector4f;

public class RenderUtils {
    public static Vec3d worldToScreen(Vec3d pos, Matrix4f modelViewMatrix, Matrix4f projectionMatrix) {
        MinecraftClient client = MinecraftClient.getInstance();
        Camera camera = client.gameRenderer.getCamera();
        Vec3d camPos = camera.getPos();
        
        Vector4f vec = new Vector4f(
            (float) (pos.x - camPos.x),
            (float) (pos.y - camPos.y),
            (float) (pos.z - camPos.z),
            1.0f
        );

        vec.mul(modelViewMatrix);
        vec.mul(projectionMatrix);

        if (vec.w <= 0.0f) return null;

        float invW = 1.0f / vec.w;
        vec.x *= invW;
        vec.y *= invW;

        float x = (vec.x + 1.0f) * 0.5f * client.getWindow().getFramebufferWidth();
        float y = (1.0f - vec.y) * 0.5f * client.getWindow().getFramebufferHeight();

        return new Vec3d(x, y, vec.w);
    }
}
