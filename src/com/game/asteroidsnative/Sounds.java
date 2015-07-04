package com.game.asteroidsnative;

import java.io.IOException;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;
import android.widget.Toast;

public class Sounds 
{
	SoundPool soundPool;
	boolean soundLoaded = false;
	AssetManager assets;
	int shootSound, breakSound,patrolSound;
	int [] sound = new int[7];
	float [] volume = new float[7];
	MainActivity activity;
	public Sounds(MainActivity _activity)
	{
		activity= _activity;
		loadSounds();
	}
	int nsounds =0;
	@SuppressWarnings("deprecation")
	private void loadSounds()
	{
		soundPool = new SoundPool(3, AudioManager.STREAM_NOTIFICATION, 0);
	    soundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
	        @Override
	        public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
	          Log.d("MY", "Complete load sampleId = " + sampleId + " status = " + status);
	          if (status == 0)
	            nsounds++;
	        }
	      });
		assets = activity.getAssets();
		int shipBreakSound, endOfGameSound, explosionSound, bonusSound;
	    shootSound = loadSound("shoot.mp3");
	    breakSound = loadSound("break.mp3");
	    patrolSound = loadSound("patrol.mp3");
	    bonusSound = loadSound("bonus.mp3");
	    explosionSound = loadSound("explosion.mp3");
	    shipBreakSound = loadSound("shipbreak.mp3");
	    endOfGameSound = loadSound("aliluia.mp3");
	    sound[0] = shootSound;
	    sound[1] = breakSound;
	    sound[2] = patrolSound;
	    sound[3] = bonusSound;
	    sound[4] = explosionSound;
	    sound[5] = shipBreakSound;
	    sound[6] = endOfGameSound;
	    volume[0] = 0.2f;
	    volume[1] = 1.0f;
	    volume[2] = 1.0f;
	    volume[3] = 1.0f;
	    volume[4] = 1.0f;
	    volume[5] = 1.0f;
	    volume[6] = 1.0f;
	}
	private int loadSound(String fileName) {
	    AssetFileDescriptor afd = null;
	    try {
	      afd = assets.openFd(fileName);
	    } catch (IOException e) {
	      e.printStackTrace();
	      Toast.makeText(activity, "Couldn't load file '" + fileName + "'", Toast.LENGTH_SHORT).show();
	      return -1;
	    }
	    return soundPool.load(afd, 1);
	  }
	public void playSound(int isound) {
		  if (sound[isound] > 0)
		    soundPool.play(sound[isound], volume[isound], volume[isound], isound, 0, 1);
		}
}
