package com.game.asteroidsnative;


import java.util.Locale;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
//import android.view.Menu;
//import android.view.MenuItem;
import android.widget.Toast;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.startad.lib.SADView;
public class MainActivity extends Activity implements OnClickListener
{
	private SurfaceWrapper glSurfaceView;
	private boolean rendererSet;
	RendererWrapper renderer;
	boolean pause = false;
	boolean locale_ru = false;
	Button playButton, continueButton, topResultsButton, exitButton,nameOkButton, soundButton, musicButton;
	public int layoutId =0;
	TopResults topResults;
	int scores = 0;
	public Sounds sounds;
	public boolean soundFlag = true, musicFlag;
	private InterstitialAd interstitial;
    protected SADView sadView;
    protected LinearLayout adViewLayout;
    protected String currentAdPlaceId = AdConstants.ADPLACEID_DEFAULT;

    Handler mainHandler = new Handler() {
        public void handleMessage(Message msg) {
             // remove surfaceview from layout and show non-opengl views
        	if (msg.what == 999)
        		gameOver();
        	else if (msg.what == 888)
        	{
        		renderer.newgame = false;
        		displayInterstitial() ;
        	}
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
	        String sLanguage = Locale.getDefault().getLanguage();
	        if (sLanguage.equals("ru"))
	        	locale_ru=true;
	        glSurfaceView.setEGLContextClientVersion(2);
	        renderer = new RendererWrapper(); 
	        glSurfaceView.setRenderer(renderer);
	        renderer.setActivity(this);
	        
	        rendererSet = true;
	        //setContentView(glSurfaceView);
	        topResults = new TopResults(this);
	        interstitial = new InterstitialAd(this);
	        interstitial.setAdUnitId("ca-app-pub-5616246497492135/2242267005");
	        // Создаём запрос к AdMob
	        AdRequest adRequesti = new AdRequest.Builder().build();
	        //AdRequest adRequesti = new AdRequest.Builder()
	        //	.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
	        //	.addTestDevice("C6E7F2711495E0333E699F49859621AF")
	        //	.build();
	        interstitial.loadAd(adRequesti);

	        // Начинаем загружать объявление
	        interstitial.setAdListener(new AdListener() {
	        	@Override
	        	public void onAdLoaded() {
	        	//Toast.makeText(getBaseContext(), "Загружено", Toast.LENGTH_SHORT).show();
	        	}
	            @Override
	            public void onAdClosed() {
	                requestNewInterstitial();
	                //beginPlayingGame();
	            }
	        	});

	    } else {
	        // Should never be seen in production, since the manifest filters
	        // unsupported devices.
	        Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
	                Toast.LENGTH_LONG).show();
	        return;
	    }
        showStartScreen();
	    
	}
    protected void reloadSAD(){
//    	this.adViewLayout.bringToFront();
    	if (sadView == null)
    		return;
    	if (locale_ru)
    		this.sadView.loadAd(this.currentAdPlaceId, SADView.LANGUAGE_RU);
    	else
    		this.sadView.loadAd(this.currentAdPlaceId, SADView.LANGUAGE_EN);
//        sadView.bringToFront();
    }

	final int adPeriod = 2;
	int adCount = 0;
	public void displayInterstitial() {
		if (adCount !=0)
		{
			adCount--;
			return;
		}
		adCount = adPeriod;
		reloadSAD();
		if (interstitial.isLoaded()) {
		interstitial.show();
		}
		}
	private void requestNewInterstitial() {
	    AdRequest adRequest = new AdRequest.Builder()
	              .addTestDevice("YOUR_DEVICE_HASH")
	              .build();

	    interstitial.loadAd(adRequest);
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
            //this.showBannerActivity(AdConstants.ADPLACEID_DEFAULT);
            this.reloadSAD();

//    		musicFlag=!musicFlag;
//    		setMusicButtonText();
    	}
    	else if (v == (View) topResultsButton)
    	{
    		displayInterstitial() ;

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
		{
			this.displayInterstitial();
			showStartScreen();
		}
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
        musicButton.setText(getString(R.string.sHelpUs));
//        musicButton.setEnabled(false);
        topResultsButton.setOnClickListener(this);
        exitButton.setOnClickListener(this);
        ImageView mImageView;
        mImageView = (ImageView) findViewById(R.id.imageView1);
        mImageView.setImageResource(R.drawable.asteroids);
        TextView scoresView = (TextView) findViewById(R.id.ScoresView);
        scoresView.setText("" + GameLibJNIWrapper.scores());
        this.adViewLayout = (LinearLayout)this.findViewById(R.id.content);
        if(null != this.sadView)
        	sadView = null;
        this.sadView = new SADView(this, AdConstants.APPLICATION_ID);
        if (sadView != null)
        {
        this.sadView.setAdListener(new SADView.SADListener() {
            @Override
            public void onReceiveAd() {
                Log.d(AdConstants.TAG, "SADListener onReceiveId");
 //               BannerActivity.this.showToast("SADListener onReceiveId");
            }

            @Override
            public void onShowedAd() {
                Log.d(AdConstants.TAG, "SADListener onShowedAd");
//                BannerActivity.this.showToast("SADListener onShowedAd");
            }

            @Override
            public void onError(SADView.ErrorCode error) {
                Log.d(AdConstants.TAG, "SADListener onError " + error);
//                BannerActivity.this.showToast("SADListener onError " + error);
            }

            @Override
            public void onAdClicked() {
                Log.d(AdConstants.TAG, "SADListener onAdClicked");
 //               BannerActivity.this.showToast("SADListener onAdClicked");
            }

            @Override
            public void noAdFound() {
                Log.d(AdConstants.TAG, "SADListener noAdFound");
//                BannerActivity.this.showToast("SADListener noAdFound");
            }
        });

        this.adViewLayout.addView(this.sadView);
        this.reloadSAD();
        }
	
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
    protected void showBannerActivity(String adPlaceId){
        Intent intent = new Intent(MainActivity.this, BannerActivity.class);
        intent.putExtra(AdConstants.ADPLACEID, adPlaceId);
        this.startActivity(intent);
    }
    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        if(null != this.sadView) this.sadView.saveInstanceState(outState);
    }

    @Override
    public void onRestoreInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        if(null != this.sadView) this.sadView.restoreInstanceState(outState);
    }


}
