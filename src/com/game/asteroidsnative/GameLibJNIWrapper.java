package com.game.asteroidsnative;

import android.content.res.AssetManager;

//import android.content.Context;

public class GameLibJNIWrapper {
    static {
//    	android.os.Debug.waitForDebugger();
        System.loadLibrary("Asteroidsnative");
    }
 
    public static native void on_surface_created();
 
    public static native void on_surface_changed(int width, int height);
 
    public static native int on_draw_frame();
    
    public static native void on_touch_event(int what, int x, int y);
    
    public static native void set_pause(boolean p);

    public static native void new_game();
    
    public static native int scores();
    
    public static native int create_game();

    public static native void add_texture(AssetManager am, String filename, int kind);
   
    public static native void add_intarr_texture(int width, int height, int[] data , int kind, boolean transparentWhite);
}