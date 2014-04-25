package org.cocos2dx.cpp_empty_test;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxGameController;

import com.bda.controller.ControllerListener;
import com.bda.controller.KeyEvent;
import com.bda.controller.MotionEvent;
import com.bda.controller.StateEvent;

public class Cocos2dxMogaControllerListener implements ControllerListener
{
	private final String mVendorName = "Moga";
	private float mOldLeftThumbstickX = 0.0f;
	private float mOldLeftThumbstickY = 0.0f;
	private float mOldRightThumbstickX = 0.0f;
	private float mOldRightThumbstickY = 0.0f;
	
	private static HashMap<Integer, Integer> sKeyMap = new HashMap<Integer, Integer>();
   
	static {
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_A , Cocos2dxGameController.BUTTON_A);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_B , Cocos2dxGameController.BUTTON_B);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_X , Cocos2dxGameController.BUTTON_X);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_Y , Cocos2dxGameController.BUTTON_Y);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_L1 , Cocos2dxGameController.BUTTON_LEFT_SHOULDER);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_R1 , Cocos2dxGameController.BUTTON_RIGHT_SHOULDER);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_L2, Cocos2dxGameController.BUTTON_LEFT_TRIGGER);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_R2, Cocos2dxGameController.BUTTON_RIGHT_TRIGGER);
        sKeyMap.put(KeyEvent.KEYCODE_DPAD_UP , Cocos2dxGameController.BUTTON_DPAD_UP);
        sKeyMap.put(KeyEvent.KEYCODE_DPAD_DOWN , Cocos2dxGameController.BUTTON_DPAD_DOWN);
        sKeyMap.put(KeyEvent.KEYCODE_DPAD_LEFT , Cocos2dxGameController.BUTTON_DPAD_LEFT);
        sKeyMap.put(KeyEvent.KEYCODE_DPAD_RIGHT , Cocos2dxGameController.BUTTON_DPAD_RIGHT);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_START , Cocos2dxGameController.BUTTON_START);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_SELECT , Cocos2dxGameController.BUTTON_SELECT);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_THUMBL , Cocos2dxGameController.BUTTON_LEFT_THUMBSTICK);
        sKeyMap.put(KeyEvent.KEYCODE_BUTTON_THUMBR , Cocos2dxGameController.BUTTON_RIGHT_THUMBSTICK);
	}
	
	@Override
	public void onKeyEvent(KeyEvent event)
	{
		boolean isPressed = event.getAction() == KeyEvent.ACTION_DOWN;
		int keycode = event.getKeyCode();
		boolean isAnalog = false;
		
		if (keycode == KeyEvent.KEYCODE_BUTTON_THUMBL || keycode == KeyEvent.KEYCODE_BUTTON_THUMBR)
		{
			isAnalog = true;
		}
		
		if (sKeyMap.containsKey(keycode))
		{
			Cocos2dxGameController.onButtonEvent(mVendorName, event.getControllerId(), sKeyMap.get(keycode), isPressed, isPressed ? 1.0f : 0.0f, isAnalog);
		}
	}

	@Override
	public void onMotionEvent(MotionEvent event)
	{	
		float newLeftThumbstickX = event.getAxisValue(MotionEvent.AXIS_X);
		if (newLeftThumbstickX != mOldLeftThumbstickX)
		{
			Cocos2dxGameController.onAxisEvent(mVendorName, event.getControllerId(), Cocos2dxGameController.LEFT_THUMBSTICK_X, newLeftThumbstickX, true);
			mOldLeftThumbstickX = newLeftThumbstickX;
		}
		
		float newLeftThumbstickY = event.getAxisValue(MotionEvent.AXIS_Y);
		if (newLeftThumbstickY != mOldLeftThumbstickY)
		{
			Cocos2dxGameController.onAxisEvent(mVendorName, event.getControllerId(), Cocos2dxGameController.LEFT_THUMBSTICK_Y, newLeftThumbstickY, true);
			mOldLeftThumbstickY = newLeftThumbstickY;
		}
		
		float newRightThumbstickX = event.getAxisValue(MotionEvent.AXIS_Z);
		if (newRightThumbstickX != mOldRightThumbstickX)
		{
			Cocos2dxGameController.onAxisEvent(mVendorName, event.getControllerId(), Cocos2dxGameController.RIGHT_THUMBSTICK_X, newRightThumbstickX, true);
			mOldRightThumbstickX = newRightThumbstickX;
		}
		
		float newRightThumbstickY = event.getAxisValue(MotionEvent.AXIS_RZ);
		if (newRightThumbstickY != mOldRightThumbstickY)
		{
			Cocos2dxGameController.onAxisEvent(mVendorName, event.getControllerId(), Cocos2dxGameController.RIGHT_THUMBSTICK_Y, newRightThumbstickY, true);
			mOldRightThumbstickY = newRightThumbstickY;
		}
	}

	@Override
	public void onStateEvent(StateEvent event)
	{
		switch(event.getState())
		{
			case StateEvent.STATE_CONNECTION:
				switch(event.getAction())
				{
					case StateEvent.ACTION_DISCONNECTED:
						// disconnected from controller
						Cocos2dxGameController.onDisconnected(mVendorName, event.getControllerId());
						break;
					case StateEvent.ACTION_CONNECTED: 
						// connected to controller
						Cocos2dxGameController.onConnected(mVendorName, event.getControllerId());
						break;
					case StateEvent.ACTION_CONNECTING:
					// attempting to connect to controller 
						break;
				} 
				break;
			case StateEvent.STATE_POWER_LOW:
				if(event.getAction() == StateEvent.ACTION_TRUE)
				{
					// controller has entered low power state
				} 
				else
				{
					// controller has entered normal power state
				} 
				break;
		}
	}
}