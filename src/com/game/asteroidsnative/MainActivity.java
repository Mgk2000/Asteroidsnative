package com.game.asteroidsnative;


import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
//import android.view.Menu;
//import android.view.MenuItem;
import android.widget.Toast;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import com.google.android.gms.ads.*;
public class MainActivity extends Activity implements OnClickListener
{
	private SurfaceWrapper glSurfaceView;
	private boolean rendererSet;
	RendererWrapper renderer;
	boolean pause = false;
	Button playButton, continueButton, topResultsButton, exitButton,nameOkButton, soundButton, musicButton;
	public int layoutId =0;
	TopResults topResults;
	int scores = 0;
	public Sounds sounds;
	public boolean soundFlag = true, musicFlag;
	private InterstitialAd interstitial;
    Handler mainHandler = new Handler() {
        public void handleMessage(Message msg) {
             // remove surfaceview from layout and show non-opengl views
        	gameOver();
        }
    };
	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	 
	    ActivityManager activityManager
	        = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
	    ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
	    final boolean supportsEs2 =
	        configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();
	    sounds = new Sounds(this);
	    if (supportsEs2) {
	        glSurfaceView = new SurfaceWrapper(this);
	 
	        if (isProbablyEmulator()) {
	            // Avoids crashes on startup with some emulator images.
	            glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
	        }
	 
	        glSurfaceView.setEGLContextClientVersion(2);
	        renderer = new RendererWrapper(); 
	        glSurfaceView.setRenderer(renderer);
	        renderer.setActivity(this);
	        
	        rendererSet = true;
	        //setContentView(glSurfaceView);
	        topResults = new TopResults(this);
	        showStartScreen();

	    } else {
	        // Should never be seen in production, since the manifest filters
	        // unsupported devices.
	        Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
	                Toast.LENGTH_LONG).show();
	        return;
	    }
	}

	private boolean isProbablyEmulator() {
	    return  Build.VERSION.SDK_INT >= 15
	            && (Build.FINGERPRINT.startsWith("generic")
	                    || Build.FINGERPRINT.startsWith("unknown")
	                    || Build.MODEL.contains("google_sdk")
	                    || Build.MODEL.contains("Emulator")
	                    || Build.MODEL.contains("Android SDK built for x86"));
	}
	@Override
	public void setContentView(int layoutResID) {
	    this.layoutId = layoutResID;
	    super.setContentView(layoutResID);
	}
	@Override
	public void setContentView(View view) {
	    this.layoutId = 777;
	    super.setContentView(view);
	}
    public void onClick(View v)
    {
        //Toast.makeText(this, "It works", Toast.LENGTH_SHORT).show();
    	if (v == (View) playButton)
    	{
    		//GameLibJNIWrapper.new_game();
    		renderer.newgame = true;
    		pause = false;
    		setContentView(glSurfaceView);
    	}
    	else if (v == (View) continueButton)
    	{
    		pause = false;
    		renderer.newgame = false;
    		setContentView(glSurfaceView);
    	}
    	else if (v == (View) soundButton)
    	{
    		soundFlag = !soundFlag;
    		setSoundButtonText();
    	}
    	else if (v == (View) musicButton)
    	{
    		musicFlag=!musicFlag;
    		setMusicButtonText();
    	}
    	else if (v == (View) topResultsButton)
    	{
    		topResults.show();
    	}
    	else if (v == (View) nameOkButton)
    	{
    		hideKeyboard();
    		EditText nameEdit = (EditText) findViewById(R.id.NameEdit);
    		String name = nameEdit.getText().toString();
    		topResults.saveResult(name,scores);
    		topResults.show();
    	}
    	else if (v == (View) exitButton)
    	{
    		finish();
    		System.exit(0);
    	}

    }
    void setSoundButtonText()
    {
		if (soundFlag)
			soundButton.setText(getString(R.string.sSoundOn));
		else
			soundButton.setText(getString(R.string.sSoundOff));
    }
    void setMusicButtonText()
    {
		if (musicFlag)
			soundButton.setText(getString(R.string.sMusicOn));
		else
			musicButton.setText(R.string.sMusicOff);
    }
	@Override
	protected void onPause() {
	    super.onPause();
	 
	    if (rendererSet) {
	        glSurfaceView.onPause();
	    }
	}
	 
	@Override
	protected void onResume() {
	    super.onResume();
	 
	    if (rendererSet) {
	        glSurfaceView.onResume();
	    }
	}
/*	@Override
	public boolean dispatchKeyEvent (KeyEvent event) {
	    if (event.getAction()==KeyEvent.ACTION_DOWN && event.getKeyCode()==KeyEvent.KEYCODE_BACK) {
	        Toast.makeText(this, "Back button pressed", Toast.LENGTH_LONG).show();
	        onBackPressed();
	        return true;
	    }
	    return false;
	}
	@Override
	   public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if (keyCode == KeyEvent.KEYCODE_BACK)
		{
		    onBackPressed();
		}
	
		return super.onKeyDown(keyCode, event);
	}*/
	@Override
	public void onBackPressed()
	{
//		return;
		if (layoutId ==0 || layoutId == R.layout.activity_main)
		{
    		finish();
    		System.exit(0);
		}
		else if (layoutId == R.layout.topresults)
		{
			showStartScreen();
		}
		else
		{
			pause = true;
			GameLibJNIWrapper.set_pause(true);
			showStartScreen();
		}
	}
	void gameOver()
	{
		scores = GameLibJNIWrapper.scores();
		if (topResults.isTopResult(scores))
		{
			askName();
		}
		else	
			showStartScreen();
	}
	public void showStartScreen()
	{
		setContentView(R.layout.activity_main);
        playButton = (Button) findViewById(R.id.NewGameButton);
        continueButton = (Button) findViewById(R.id.ContinueButton);
        soundButton = (Button) findViewById(R.id.SoundButton);
        musicButton = (Button) findViewById(R.id.MusicButton);
        topResultsButton = (Button) findViewById(R.id.TopResultsButton);
        exitButton = (Button) findViewById(R.id.ExitButton);
        playButton.setOnClickListener(this);
        continueButton.setOnClickListener(this);
        continueButton.setEnabled(pause);
        setSoundButtonText();
        setMusicButtonText();
        soundButton.setOnClickListener(this);
        musicButton.setOnClickListener(this);
        musicButton.setEnabled(false);
        topResultsButton.setOnClickListener(this);
        exitButton.setOnClickListener(this);
        ImageView mImageView;
        mImageView = (ImageView) findViewById(R.id.imageView1);
        mImageView.setImageResource(R.drawable.asteroid);
        TextView scoresView = (TextView) findViewById(R.id.ScoresView);
        scoresView.setText("" + GameLibJNIWrapper.scores());
	
	}
	void askName()
	{
		//showInputDialog();
		//return strCatName;
		setContentView(R.layout.inputname);
		nameOkButton =  (Button) findViewById(R.id.NameOkButton);
		nameOkButton.setOnClickListener(this);

		
	}
	public void hideKeyboard() 
	{
	    InputMethodManager inputMethodManager = 
	    		(InputMethodManager) getSystemService(Activity.INPUT_METHOD_SERVICE);
	    //Find the currently focused view, so we can grab the correct window token from it.
	    View view = getCurrentFocus();
	    //If no view currently has focus, create a new one, just so we can grab a window token from it
	    if(view == null) {
	        view = new View(this);
	    }
	    inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
	}
	void resume()
	{
		
	}
	public void playSound(int ind)
	{
		sounds.playSound(ind);
	}
}
