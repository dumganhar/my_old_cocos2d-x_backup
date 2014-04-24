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

import java.util.HashMap;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGameController;
import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;
import com.bda.controller.KeyEvent;
import com.bda.controller.MotionEvent;
import com.bda.controller.StateEvent;
import android.os.Bundle;
import android.os.Handler;

class MogaPro2ControllerListener implements ControllerListener
{
	private final String mVenderName = "Moga";
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
			Cocos2dxGameController.onButtonEvent(mVenderName, event.getControllerId(), sKeyMap.get(keycode), isPressed, isPressed ? 1.0f : 0.0f, isAnalog);
		}
	}

	@Override
	public void onMotionEvent(MotionEvent event)
	{	
		float newLeftThumbstickX = event.getAxisValue(MotionEvent.AXIS_X);
		if (newLeftThumbstickX != mOldLeftThumbstickX)
		{
			Cocos2dxGameController.onAxisEvent(mVenderName, event.getControllerId(), Cocos2dxGameController.LEFT_THUMBSTICK_X, newLeftThumbstickX, true);
			mOldLeftThumbstickX = newLeftThumbstickX;
		}
		
		float newLeftThumbstickY = event.getAxisValue(MotionEvent.AXIS_Y);
		if (newLeftThumbstickY != mOldLeftThumbstickY)
		{
			Cocos2dxGameController.onAxisEvent(mVenderName, event.getControllerId(), Cocos2dxGameController.LEFT_THUMBSTICK_Y, newLeftThumbstickY, true);
			mOldLeftThumbstickY = newLeftThumbstickY;
		}
		
		float newRightThumbstickX = event.getAxisValue(MotionEvent.AXIS_Z);
		if (newRightThumbstickX != mOldRightThumbstickX)
		{
			Cocos2dxGameController.onAxisEvent(mVenderName, event.getControllerId(), Cocos2dxGameController.RIGHT_THUMBSTICK_X, newRightThumbstickX, true);
			mOldRightThumbstickX = newRightThumbstickX;
		}
		
		float newRightThumbstickY = event.getAxisValue(MotionEvent.AXIS_RZ);
		if (newRightThumbstickY != mOldRightThumbstickY)
		{
			Cocos2dxGameController.onAxisEvent(mVenderName, event.getControllerId(), Cocos2dxGameController.RIGHT_THUMBSTICK_Y, newRightThumbstickY, true);
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
						Cocos2dxGameController.onDisconnected(mVenderName, event.getControllerId());
						break;
					case StateEvent.ACTION_CONNECTED: 
						// connected to controller
						Cocos2dxGameController.onConnected(mVenderName, event.getControllerId());
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

public class AppActivity extends Cocos2dxActivity {
	
	private Controller mController = null;
	private MogaPro2ControllerListener mMogaPro2Listener = new MogaPro2ControllerListener();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mController = Controller.getInstance(this);
		mController.init();
		mController.setListener(mMogaPro2Listener, new Handler());
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
		if(mController != null) {
			mController.onPause(); 
		}
	}
	@Override
	protected void onResume() {
		super.onResume();
		if(mController != null) {
			mController.onResume();
		}
	}
	
	@Override
	protected void onDestroy() {
		if(mController != null)
		{
			mController.exit();
		}
		super.onDestroy();
	}
	
}
