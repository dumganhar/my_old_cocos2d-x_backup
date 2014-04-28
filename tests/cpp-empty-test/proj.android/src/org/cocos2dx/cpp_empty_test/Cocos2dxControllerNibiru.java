package org.cocos2dx.cpp_empty_test;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxGameController;

import com.nibiru.lib.controller.AccEvent;
import com.nibiru.lib.controller.Controller;
import com.nibiru.lib.controller.ControllerDevice;
import com.nibiru.lib.controller.ControllerKeyEvent;
import com.nibiru.lib.controller.ControllerService;
import com.nibiru.lib.controller.ControllerService.OnControllerSeviceListener;
import com.nibiru.lib.controller.ControllerServiceException;
import com.nibiru.lib.controller.GyroEvent;
import com.nibiru.lib.controller.OnAccListener;
import com.nibiru.lib.controller.OnGyroListener;
import com.nibiru.lib.controller.OnKeyListener;
import com.nibiru.lib.controller.OnSimpleStickListener;
import com.nibiru.lib.controller.OnStateListener;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class Cocos2dxControllerNibiru implements OnControllerSeviceListener, OnKeyListener, OnSimpleStickListener, OnAccListener, OnGyroListener, OnStateListener {

	private String TAG = "NibiruTag";
	private String mVendorName = "Nibiru";
	private ControllerService mControllerService = null;
	private Context mContext = null;
	
	private HashMap<Integer, Integer> mKeyMap = new HashMap<Integer, Integer>();
	   
	Cocos2dxControllerNibiru() {
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_A , Cocos2dxGameController.BUTTON_A);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_B , Cocos2dxGameController.BUTTON_B);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_X , Cocos2dxGameController.BUTTON_X);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_Y , Cocos2dxGameController.BUTTON_Y);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_L1 , Cocos2dxGameController.BUTTON_LEFT_SHOULDER);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_R1 , Cocos2dxGameController.BUTTON_RIGHT_SHOULDER);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_L2, Cocos2dxGameController.BUTTON_LEFT_TRIGGER);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_R2, Cocos2dxGameController.BUTTON_RIGHT_TRIGGER);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_UP , Cocos2dxGameController.BUTTON_DPAD_UP);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_DOWN , Cocos2dxGameController.BUTTON_DPAD_DOWN);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_LEFT , Cocos2dxGameController.BUTTON_DPAD_LEFT);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_RIGHT , Cocos2dxGameController.BUTTON_DPAD_RIGHT);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_START , Cocos2dxGameController.BUTTON_START);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_SELECT , Cocos2dxGameController.BUTTON_SELECT);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_THUMBL , Cocos2dxGameController.BUTTON_LEFT_THUMBSTICK);
        mKeyMap.put(ControllerKeyEvent.KEYCODE_BUTTON_THUMBR , Cocos2dxGameController.BUTTON_RIGHT_THUMBSTICK);
	}
	public void onCreate(Context context) {
		mContext = context;
		
		mControllerService = Controller.getControllerService();
	}
	
	public void onPause() {
		mControllerService.setEnable(false);
	}
	
	public void onResume() {
		if( mControllerService.checkNibiruInstall(mContext, true) ) {
			try {
				mControllerService.setControllerServiceListener(this);
				mControllerService.setStateListener(this);
				mControllerService.setKeyListener(this);
				mControllerService.setSimpleStickListener(this);
				mControllerService.setAccListener(this);
				mControllerService.setGyroListener(this);
				mControllerService.setEnableLR2(true);
				mControllerService.setAutoKeyUpMode(false);
				mControllerService.register(mContext);
			} catch (ControllerServiceException e) {
				e.printStackTrace();
			} 
		}
	}
	
	public void onDestroy() {
		if( mControllerService != null ){
			mControllerService.unregister();
		}
	}

	@Override
	public void onControllerServiceReady(boolean isSucc) {
		if( isSucc )
		{
			if( !mControllerService.hasDeviceConnected() ){
				Bundle bun = new Bundle();
				bun.putBoolean(ControllerService.FLAG_IS_SHOW_GAMEPAD_TIP, true);
				try {
					mControllerService.showDeviceManagerUI(mContext, bun); 
				} catch (ControllerServiceException e) {
						e.printStackTrace(); 
				}
			} 
		}
	}

	@Override
	public void onControllerKeyDown(int playerOrder, int keyCode, ControllerKeyEvent event) {
		
		Log.d(TAG, "KeyDown: " + keyCode);
		if (!mKeyMap.containsKey(keyCode)) {
			Log.e(TAG, "Didn't map the key: " + keyCode);
		}
		
		Cocos2dxGameController.onButtonEvent(mVendorName, playerOrder, mKeyMap.get(keyCode), true, 1.0f, false);
	}

	@Override
	public void onControllerKeyUp(int playerOrder, int keyCode, ControllerKeyEvent event) {
		
		Log.d(TAG, "Keyup: " + keyCode);
		
		if (!mKeyMap.containsKey(keyCode)) {
			Log.e(TAG, "Didn't map the key: " + keyCode);
		}
		
		Cocos2dxGameController.onButtonEvent(mVendorName, playerOrder, mKeyMap.get(keyCode), false, 0.0f, false);
	}

	@Override
	public void onLeftStickChanged(int playerOrder, float x, float y) {
		
	}

	@Override
	public void onRightStickChanged(int playerOrder, float x, float y) {
		
	}

	@Override
	public void onControllerAccEvent(int playerOrder, AccEvent event) {
		
	}

	@Override
	public void onControllerGyroEvent(int playerOrder, GyroEvent event) {
		
	}

	@Override
	public void onBluetoothStateChanged(int state) {
		
	}

	@Override
	public void onControllerStateChanged(int playerOrder, int state, ControllerDevice device) {
		if (state == ControllerDevice.STATE_CONN)
		{
			Cocos2dxGameController.onConnected(mVendorName, playerOrder);
		}
		else if (state == ControllerDevice.STATE_DISCONN)
		{
			Cocos2dxGameController.onDisconnected(mVendorName, playerOrder);
		}
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return mControllerService.handleExternalInput(event);
	}
	
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		return mControllerService.handleExternalInput(event);
	}
	
	public boolean onGenericMotionEvent(MotionEvent event) {
		return mControllerService.handleExternalInput(event);
	}
}
