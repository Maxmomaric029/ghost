package com.ejemplo.cheatmod.core;

import java.util.ArrayList;
import java.util.List;

public abstract class Module {
    private final String name;
    private final Category category;
    private boolean enabled;
    private int keyCode;
    protected final List<Setting<?>> settings = new ArrayList<>();

    public Module(String name, Category category) {
        this.name = name;
        this.category = category;
    }

    public void toggle() {
        setEnabled(!enabled);
    }

    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
        if (enabled) onEnable();
        else onDisable();
    }

    public abstract void onEnable();
    public abstract void onDisable();
    public void onTick() {}
    
    public String getName() { return name; }
    public boolean isEnabled() { return enabled; }
    public Category getCategory() { return category; }
    public int getKeyCode() { return keyCode; }
    public void setKeyCode(int keyCode) { this.keyCode = keyCode; }
    public List<Setting<?>> getSettings() { return settings; }

    public enum Category {
        COMBAT, RENDER, PLAYER, MISC
    }
}
