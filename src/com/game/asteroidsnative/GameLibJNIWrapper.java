package com.game.asteroidsnative;

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
   
}