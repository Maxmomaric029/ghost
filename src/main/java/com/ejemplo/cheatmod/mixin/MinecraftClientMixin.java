package com.ejemplo.cheatmod.mixin;

import com.ejemplo.cheatmod.CheatMod;
import net.minecraft.client.MinecraftClient;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(MinecraftClient.class)
public class MinecraftClientMixin {
    @Inject(method = "tick", at = @At("END"))
    private void onTick(CallbackInfo ci) {
        if (CheatMod.MODULE_MANAGER != null) {
            CheatMod.MODULE_MANAGER.onTick();
        }
    }
}
