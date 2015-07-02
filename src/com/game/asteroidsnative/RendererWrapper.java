package com.game.asteroidsnative;
import javax.microedition.khronos.opengles.GL10;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Message;
public class RendererWrapper implements Renderer {
	MainActivity activity;
	void setActivity(MainActivity _activity)
	{
		activity = _activity;
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
        }
    }

	@Override
	public void onSurfaceCreated(GL10 gl,
			javax.microedition.khronos.egl.EGLConfig config) 
	{
		GameLibJNIWrapper.create_game();
    	AssetManager am = activity.getAssets();
    	GameLibJNIWrapper.add_texture(am,"Asteroidsurface.bmp", 0);
    	GameLibJNIWrapper.add_texture(am,"SuperGun.bmp", 4);
    	GameLibJNIWrapper.add_texture(am,"Diamond.bmp", 5);
    	GameLibJNIWrapper.add_texture(am,"LittleBomb.bmp", 1);
    	GameLibJNIWrapper.add_texture(am,"BigBomb.bmp", 2);
    	GameLibJNIWrapper.add_texture(am,"Live.bmp", 3);
        GameLibJNIWrapper.on_surface_created(); 
	}
	public void playSound (int ind)
	{
		if (activity.soundFlag);
			activity.sounds.playSound(ind);
	}
}