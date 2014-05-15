/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp_empty_test;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;


public class AppActivity extends Activity {
   
	private static final String TAG = "AppActivity";
    public static final int DIALOG_DOWNLOAD_PROGRESS = 0;
    private Button startBtn;
    private ProgressDialog mProgressDialog;
    private final String mUrl = "https://raw.githubusercontent.com/dumganhar/bin_for_downloading/master/libcpp_empty_test.so";
    private String mFilePathToStore = null;
    private Handler mHandler = new Handler();

    
    public void startGameActivity() {
		try{
		//Starting a new Intent
        Intent nextScreen = new Intent(getApplicationContext(),  GameActivity.class);
        startActivity(nextScreen);
		} catch (Exception e)
		{
			e.printStackTrace();
		}
    }
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
		PackageManager pm = getPackageManager();
		String dataDir = null;
		try {
			dataDir = pm.getApplicationInfo(getPackageName(), 0).dataDir;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		mFilePathToStore = dataDir + "/libcpp_empty_test.so";
		
        setContentView(R.layout.main);
        startBtn = (Button)findViewById(R.id.startBtn);
        startBtn.setOnClickListener(new OnClickListener(){
            public void onClick(View v) {
            	// Check whether the so file exists
               	File file = new File(mFilePathToStore);
                if (!file.exists() || file.length() != 4324456) {
            		startDownload();
                }
            	else {
            		startGameActivity();
            	}
            		
            }
        });
        
        Log.d(TAG, "AppActivity.onCreate");
    }

    private void startDownload() {
        new DownloadFileAsync().execute(mUrl);
    }
    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
		case DIALOG_DOWNLOAD_PROGRESS:
			mProgressDialog = new ProgressDialog(this);
			mProgressDialog.setMessage("Downloading file..");
			mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			mProgressDialog.setCancelable(false);
			mProgressDialog.show();
			return mProgressDialog;
		default:
			return null;
        }
    }

	class DownloadFileAsync extends AsyncTask<String, String, String> {
	   
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			showDialog(DIALOG_DOWNLOAD_PROGRESS);
		}
	
		@Override
		protected String doInBackground(String... aurl) {
			int count;
	
		try {
	
		URL url = new URL(aurl[0]);
		URLConnection conexion = url.openConnection();
		conexion.connect();
	
		int lenghtOfFile = conexion.getContentLength();
		Log.d("ANDRO_ASYNC", "Lenght of file: " + lenghtOfFile);
	
		InputStream input = new BufferedInputStream(url.openStream());
		OutputStream output = new FileOutputStream(mFilePathToStore);
	
		byte data[] = new byte[1024];
	
		long total = 0;
	
			while ((count = input.read(data)) != -1) {
				total += count;
				publishProgress(""+(int)((total*100)/lenghtOfFile));
				output.write(data, 0, count);
			}
	
			output.flush();
			output.close();
			input.close();
		} catch (Exception e) {}
		return null;
	
		}
		protected void onProgressUpdate(String... progress) {
			 Log.d("ANDRO_ASYNC",progress[0]);
			 mProgressDialog.setProgress(Integer.parseInt(progress[0]));
		}
	
		@Override
		protected void onPostExecute(String unused) {
			dismissDialog(DIALOG_DOWNLOAD_PROGRESS);
			startGameActivity();
		}
	}
}
