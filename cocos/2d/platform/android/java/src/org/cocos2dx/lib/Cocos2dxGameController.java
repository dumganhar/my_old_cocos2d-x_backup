package org.cocos2dx.lib;

public class Cocos2dxGameController {
	public static final int LEFT_THUMBSTICK_X = 101;
	public static final int LEFT_THUMBSTICK_Y = 102;
	public static final int RIGHT_THUMBSTICK_X = 103;
	public static final int RIGHT_THUMBSTICK_Y = 104;
	public static final int BUTTON_A = 105;
	public static final int BUTTON_B = 106;
	public static final int BUTTON_X = 107;
	public static final int BUTTON_Y = 108;
	public static final int BUTTON_LEFT_SHOULDER = 109;
	public static final int BUTTON_RIGHT_SHOULDER = 110;
	public static final int BUTTON_DPAD_UP = 111;
	public static final int BUTTON_DPAD_DOWN = 112;
	public static final int BUTTON_DPAD_LEFT = 113;
	public static final int BUTTON_DPAD_RIGHT = 114;
	public static final int BUTTON_START = 115;
	public static final int BUTTON_SELECT = 116;
	public static final int BUTTON_LEFT_THUMBSTICK = 117;
	public static final int BUTTON_RIGHT_THUMBSTICK = 118;
	
	
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
