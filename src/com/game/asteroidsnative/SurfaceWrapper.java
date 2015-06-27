package com.game.asteroidsnative;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class SurfaceWrapper extends GLSurfaceView 
{	
	public SurfaceWrapper(Context context, AttributeSet attrs) 
	{
		super(context, attrs);		
		this.requestFocus();
		this.setFocusableInTouchMode(true);
	}

	public SurfaceWrapper(Context context) 
	{
		super(context);		
	}
	@Override
	public boolean onTouchEvent(MotionEvent event) 
	{
		if (event != null)
		{
			int iact = event.getActionMasked();
			if (iact == MotionEvent.ACTION_POINTER_DOWN || iact == MotionEvent.ACTION_DOWN)
				iact = 1;
			if (iact == 1 || iact == MotionEvent.ACTION_MOVE)
			{
				int np = event.getPointerCount ();
				for (int i =0; i< np; i++)
				{
					float x = event.getX(i);
					float y = event.getY(i);
					if (np == 2)
						Log.d("Aster", String.format("x=%f,  y=%f iact = %d",x, y, iact ));
					GameLibJNIWrapper.on_touch_event(iact, (int) x, (int) y);
				}
			}

			return true;
		}
		else
		{
			return super.onTouchEvent(event);
		}		
	}

}
