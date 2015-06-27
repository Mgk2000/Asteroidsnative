package com.game.asteroidsnative;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;

import android.app.ActionBar.LayoutParams;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

class Result
{
	String name;
	int res;
	
}

public class TopResults implements OnClickListener 
{
	final int NRESULTS = 10;
	List<Result> results = new ArrayList<Result>();
	MainActivity activity;
	Button  backButton;
	TopResults(MainActivity _activity)
	{
		activity = _activity;
		readResults();
	}
	void readResults()
	{
		File dir = activity.getFilesDir();
		File file = new File(dir, "results.txt");
		FileInputStream ifstream;
		try {
			ifstream = new FileInputStream(file);
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			return;
		}
        BufferedReader in = null;
        try 
        {
        	in = new BufferedReader(
        		new InputStreamReader(ifstream));
            for (int i = 0; i < NRESULTS; i++) 
            {
           		String name = in.readLine();
           		if (name == null)
           			break;
            	String sres = in.readLine();
           		if (sres == null)
           			break;
            	Result res = new Result();
            	res.name = name;
            	res.res = Integer.parseInt(sres);
                results.add(res);
            }
        }
        catch (IOException e) 
        {
            // :( It's ok we have defaults
        }
        catch (NumberFormatException e) 
        {
            // :/ It's ok, defaults save our day
        } finally {
            try
            {
                if (in != null)
                    in.close();
            } 
            catch (IOException e)
            {
            }
        }
    }
	boolean saveResult(String name, int res)
	{
		Result newres = new Result();
		newres.name = name;
		newres.res = res;
		boolean found = false;
		for (int i=0; i< results.size(); i++)
			if (results.get(i).res < res)
			{
				results.add(i, newres);
				found = true;
				break;
			}
		if (found)
		{
			if  (results.size()>NRESULTS)
				results.remove(NRESULTS);
		}
		else
		{
			if (results.size() < NRESULTS)
				results.add(newres);
			else 
				return false;
		}
		File file = new File(activity.getFilesDir(), "results.txt");
		FileOutputStream ofstream;
		try {
			ofstream = new FileOutputStream(file);
		} catch (FileNotFoundException e1) {
			return false;
		}

		BufferedWriter out = null;
		try
		{
			out = new BufferedWriter(new OutputStreamWriter(
					ofstream));
			for (int i = 0; i < results.size(); i++)
			{
				out.write(results.get(i).name +"\n");
				out.write(Integer.toString(results.get(i).res) +"\n");
			}
		}
		catch (IOException e) 
		{
		}
		finally
		{
			try
			{
				if (out != null)
					out.close();
			} 
			catch (IOException e)
			{
			}
		}
		return true;
		
	}
	public boolean isTopResult(int res)
	{
		if (results.size()< NRESULTS)
			return true;
		for (int i=0; i< NRESULTS; i++)
			if (res> results.get(i).res)
			return true;
		return false;
	}
	public void show ()
	{
		activity.setContentView(R.layout.topresults);
		backButton = (Button)activity.findViewById(R.id.BackButton);
		backButton.setOnClickListener(this);
		LinearLayout ll = (LinearLayout) activity.findViewById(R.id.TopResultsLayout);
		int textSize = 18;
		for (int i =0; i< results.size(); i++)
		{
			LinearLayout rl = new LinearLayout(activity);
			LayoutParams lparams=new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
			rl.setLayoutParams(lparams);
			TextView tv = new TextView(activity);
			tv.setText((i+1) + " ");
			tv.setTextSize(textSize);  
            //tv.setGravity(Gravity.LEFT);
			tv.setLayoutParams(new LinearLayout.LayoutParams
					(0, LayoutParams.WRAP_CONTENT, 1.0f));	
			rl.addView(tv);
			
			tv = new TextView(activity);
			tv.setText(Integer.toString(results.get(i).res));
			tv.setTextSize(textSize);  
			tv.setLayoutParams(new LinearLayout.LayoutParams
					(0, LayoutParams.WRAP_CONTENT, 3.0f));	
			rl.addView(tv);
			
			tv = new TextView(activity);
			tv.setText(results.get(i).name);
			tv.setTextSize(textSize);  
			tv.setLayoutParams(new LinearLayout.LayoutParams
					(0, LayoutParams.WRAP_CONTENT, 6.0f));	
			rl.addView(tv);
			ll.addView(rl);
		}
	}
    public void onClick(View v)
    {
    	if (v == (View) backButton)
    	{
    		activity.showStartScreen();
    	}
    }

}
