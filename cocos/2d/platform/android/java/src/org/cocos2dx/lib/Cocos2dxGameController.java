package org.cocos2dx.lib;

import java.util.ArrayList;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class Cocos2dxGameController {
	public static final int LEFT_THUMBSTICK_X = MotionEvent.AXIS_X;
	public static final int LEFT_THUMBSTICK_Y = MotionEvent.AXIS_Y;
	public static final int RIGHT_THUMBSTICK_X = MotionEvent.AXIS_RX;
	public static final int RIGHT_THUMBSTICK_Y = MotionEvent.AXIS_RY;
	public static final int BUTTON_A = KeyEvent.KEYCODE_BUTTON_A;
	public static final int BUTTON_B = KeyEvent.KEYCODE_BUTTON_B;
	public static final int BUTTON_X = KeyEvent.KEYCODE_BUTTON_X;
	public static final int BUTTON_Y = KeyEvent.KEYCODE_BUTTON_Y;
	public static final int BUTTON_LEFT_SHOULDER = KeyEvent.KEYCODE_BUTTON_L1;
	public static final int BUTTON_RIGHT_SHOULDER = KeyEvent.KEYCODE_BUTTON_R1;
	public static final int BUTTON_DPAD_UP = KeyEvent.KEYCODE_DPAD_UP;
	public static final int BUTTON_DPAD_DOWN = KeyEvent.KEYCODE_DPAD_DOWN;
	public static final int BUTTON_DPAD_LEFT = KeyEvent.KEYCODE_DPAD_LEFT;
	public static final int BUTTON_DPAD_RIGHT = KeyEvent.KEYCODE_DPAD_RIGHT;
	public static final int BUTTON_START = KeyEvent.KEYCODE_BUTTON_START;
	public static final int BUTTON_SELECT = KeyEvent.KEYCODE_BUTTON_SELECT;
	public static final int BUTTON_LEFT_THUMBSTICK = KeyEvent.KEYCODE_BUTTON_THUMBL;
	public static final int BUTTON_RIGHT_THUMBSTICK = KeyEvent.KEYCODE_BUTTON_THUMBR;
	public static final int BUTTON_LEFT_TRIGGER = KeyEvent.KEYCODE_BUTTON_L2;
	public static final int BUTTON_RIGHT_TRIGGER = KeyEvent.KEYCODE_BUTTON_R2;
	
	private static ArrayList<Runnable> sRunnableFrameStartList = null;
	private static ArrayList<Cocos2dxGameControllerProduct> sControllerProducts = new ArrayList<Cocos2dxGameControllerProduct>();
	
	public static void addRunnableToFrameStartList(Runnable runnable) {
		if (sRunnableFrameStartList == null)
			sRunnableFrameStartList = new ArrayList<Runnable>();
		sRunnableFrameStartList.add(runnable);
	}

	public static void removeRunnableFromFrameStartList(Runnable runnable) {
		if (sRunnableFrameStartList != null)
			sRunnableFrameStartList.remove(runnable);
	}

	public static void onDrawFrameStart() {
		if (sRunnableFrameStartList != null)
		{
			int size = sRunnableFrameStartList.size();
			for (int i = 0; i < size; ++i) {
				sRunnableFrameStartList.get(i).run();
			}
		}
	}
	
	
	public static void checkConnectedControllers() {
		for(int deviceId: InputDevice.getDeviceIds()) {
			InputDevice device = InputDevice.getDevice(deviceId);
			Cocos2dxGameController.onConnected(vendorName, deviceId);
		}
	}
	
	public static void onConnected(final String vendorName, final int controller)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				nativeControllerConnected(vendorName, controller);
			}	
		});
	}
	
	public static void onDisconnected(final String vendorName, final int controller)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				nativeControllerDisconnected(vendorName, controller);
			}	
		});
	}
	
	public static void onButtonEvent(final String vendorName, final int controller, final int button, final boolean isPressed, final float value, final boolean isAnalog)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				nativeControllerButtonEvent(vendorName, controller, button, isPressed, value, isAnalog);
			}	
		});
	}
	
	public static void onAxisEvent(final String vendorName, final int controller, final int axis, final float value, final boolean isAnalog)
	{
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				nativeControllerAxisEvent(vendorName, controller, axis, value, isAnalog);
			}	
		});
	}
	
	private static native void nativeControllerConnected(final String vendorName, final int controller);
	private static native void nativeControllerDisconnected(final String vendorName, final int controller);
	private static native void nativeControllerButtonEvent(final String vendorName, final int controller, final int button, final boolean isPressed, final float value, final boolean isAnalog);
	private static native void nativeControllerAxisEvent(final String vendorName, final int controller, final int axis, final float value, final boolean isAnalog);
}
