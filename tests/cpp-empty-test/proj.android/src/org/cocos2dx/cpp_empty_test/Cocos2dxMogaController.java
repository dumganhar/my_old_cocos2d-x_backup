package org.cocos2dx.cpp_empty_test;

import android.content.Context;
import android.os.Handler;

import com.bda.controller.Controller;

public class Cocos2dxMogaController {

	private Controller mController = null;
	private Cocos2dxMogaControllerListener mMogaControllerListener = new Cocos2dxMogaControllerListener();
	
	public void onCreate(Context context) {
		mController = Controller.getInstance(context);
		mController.init();
		mController.setListener(mMogaControllerListener, new Handler());
	}
	
	public void onPause() {
		mController.onPause();
	}
	
	public void onResume() {
		mController.onResume();
	}
	
	public void onDestroy() {
		mController.exit();
	}
}
