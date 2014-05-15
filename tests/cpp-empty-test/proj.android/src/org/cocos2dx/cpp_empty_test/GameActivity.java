package org.cocos2dx.cpp_empty_test;

import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.os.Bundle;


public class GameActivity extends Cocos2dxActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		PackageManager pm = getPackageManager();
		String dataDir = null;
		try {
			dataDir = pm.getApplicationInfo(getPackageName(), 0).dataDir;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try
		{
			System.load(dataDir + "/libcpp_empty_test.so");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		super.onCreate(savedInstanceState);
	}
	
//	private static boolean copyAsset(AssetManager assetManager,
//            String fromAssetPath, String toPath) {
//        InputStream in = null;
//        OutputStream out = null;
//        try {
//          in = assetManager.open(fromAssetPath);
//          new File(toPath).createNewFile();
//          out = new FileOutputStream(toPath);
//          copyFile(in, out);
//          in.close();
//          in = null;
//          out.flush();
//          out.close();
//          out = null;
//          return true;
//        } catch(Exception e) {
//            e.printStackTrace();
//            return false;
//        }
//    }
//
//    private static void copyFile(InputStream in, OutputStream out) throws IOException {
//        byte[] buffer = new byte[1024];
//        int read;
//        while((read = in.read(buffer)) != -1){
//          out.write(buffer, 0, read);
//        }
//    }
}
