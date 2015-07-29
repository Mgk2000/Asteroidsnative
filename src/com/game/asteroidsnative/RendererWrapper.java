package com.game.asteroidsnative;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.opengles.GL10;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Message;
public class RendererWrapper implements Renderer {
	MainActivity activity;
	AssetManager assets;
	public boolean newgame;
	boolean texturesCreated = false;
	void setActivity(MainActivity _activity)
	{
		activity = _activity;
		assets = activity.getAssets();
	}
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
    	GameLibJNIWrapper.on_surface_changed(width, height); 
    }
    @Override
    public void onDrawFrame(GL10 gl) {
    	int res = GameLibJNIWrapper.on_draw_frame();
        if (res<0)
        {
        	Message msg = Message.obtain();
			msg.what = 999;
        	activity.mainHandler.sendMessage(msg);
        }
        else if (res>0)
        {
        	playSound(res-1);
        	if (res==8)
        	{
            	Message msg = Message.obtain();
    			msg.what = 888;
            	activity.mainHandler.sendMessage(msg);
        	}
        }
    }

	@Override
	public void onSurfaceCreated(GL10 gl,
			javax.microedition.khronos.egl.EGLConfig config) 
	{
		if (true || newgame && !texturesCreated)
		{
			GameLibJNIWrapper.create_game();
	    	AssetManager am = activity.getAssets();
	        addPngTexture("Asteroidsurface.png", 0, true);
	        addPngTexture("Shooter.png", 1, true);
	        addPngTexture("LittleBomb.png", 2, true);
	        addPngTexture("Live.png", 3, true);
	        addPngTexture("SuperGun.png", 4, true);
	        addPngTexture("Diamond.png", 5, true);
	        addPngTexture("goldenkey.png", 6, true);
//	        addPngTexture("abcgraysquare.png", 7, true);
	        addPngTexture("abcmono.png", 7, false);
	        addPngTexture("Pause.png", 8, true);
	        addPngTexture("Continue.png", 9, true);
	        addPngTexture("Ship.png", 10, true);
	        addPngTexture("Patrol.png", 11, true);
	        texturesCreated = true;
		}
    	GameLibJNIWrapper.on_surface_created();
    	if (newgame)
    		GameLibJNIWrapper.new_game();
	}
	void addPngTexture(String fn, int kind, boolean transparentWhite)
	{
		InputStream istr;
	    Bitmap bitmap = null;
	    try
	    {
			istr = assets.open(fn);
	        bitmap = BitmapFactory.decodeStream(istr);
	        int w = bitmap.getWidth();
	        int h = bitmap.getHeight();
	        int [] pixels = new int[h * w];
	        bitmap.getPixels(pixels, 0, w, 0, 0, w, h);
	        GameLibJNIWrapper.add_intarr_texture(w,h,pixels, kind, transparentWhite);
	    }
	    catch (IOException e)
	    {
	        // handle exception
	    }
	}
	public void playSound (int ind)
	{
		if (activity.soundFlag)
			activity.sounds.playSound(ind);
	}
}