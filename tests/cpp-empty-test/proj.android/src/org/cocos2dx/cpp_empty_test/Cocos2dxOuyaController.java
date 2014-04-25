package org.cocos2dx.cpp_empty_test;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxGameController;

//import tv.ouya.console.api.OuyaController;
import android.content.Context;
import android.view.KeyEvent;
import android.view.MotionEvent;

//public class Cocos2dxOuyaController {
//
//	public static String sVendorName = "Ouya";
//	
//    private static HashMap<Integer, Integer> sKeyMap = new HashMap<Integer, Integer>();
//    
//    static {
//		sKeyMap.put(OuyaController.BUTTON_A, Cocos2dxGameController.BUTTON_B);
//		sKeyMap.put(OuyaController.BUTTON_O, Cocos2dxGameController.BUTTON_A);
//		sKeyMap.put(OuyaController.BUTTON_U, Cocos2dxGameController.BUTTON_X);
//		sKeyMap.put(OuyaController.BUTTON_Y, Cocos2dxGameController.BUTTON_Y);
//        sKeyMap.put(OuyaController.BUTTON_DPAD_DOWN, Cocos2dxGameController.BUTTON_DPAD_DOWN);
//        sKeyMap.put(OuyaController.BUTTON_DPAD_LEFT, Cocos2dxGameController.BUTTON_DPAD_LEFT);
//        sKeyMap.put(OuyaController.BUTTON_DPAD_RIGHT, Cocos2dxGameController.BUTTON_DPAD_RIGHT);
//        sKeyMap.put(OuyaController.BUTTON_DPAD_UP, Cocos2dxGameController.BUTTON_DPAD_UP);
//        sKeyMap.put(OuyaController.BUTTON_L1, Cocos2dxGameController.BUTTON_LEFT_SHOULDER);
//        sKeyMap.put(OuyaController.BUTTON_R1,  Cocos2dxGameController.BUTTON_RIGHT_SHOULDER);
//        sKeyMap.put(OuyaController.AXIS_L2, Cocos2dxGameController.BUTTON_LEFT_TRIGGER);
//        sKeyMap.put(OuyaController.AXIS_R2, Cocos2dxGameController.BUTTON_RIGHT_TRIGGER);
//        sKeyMap.put(OuyaController.AXIS_LS_X, Cocos2dxGameController.LEFT_THUMBSTICK_X);
//        sKeyMap.put(OuyaController.AXIS_LS_Y, Cocos2dxGameController.LEFT_THUMBSTICK_Y);
//        sKeyMap.put(OuyaController.AXIS_RS_X, Cocos2dxGameController.RIGHT_THUMBSTICK_X);
//        sKeyMap.put(OuyaController.AXIS_RS_Y, Cocos2dxGameController.RIGHT_THUMBSTICK_Y);
//    }
//    
//    public static void onCreate(Context context) {
//    	OuyaController.init(context);
//    	Cocos2dxGameController.addRunnableToFrameStartList(new Runnable() {
//
//			@Override
//			public void run() {
//				OuyaController.startOfFrame();
//			}
//    		
//    	});
//    }
//    
//    public static boolean onKeyDown(int keyCode, KeyEvent event) {
//        boolean handled = false;
//
//        handled = OuyaController.onKeyDown(keyCode, event);
//
//        if (handled)
//        {
//    		int keycode = event.getKeyCode();
//    		boolean isAnalog = false;
//    		
//    		if (keycode == KeyEvent.KEYCODE_BUTTON_THUMBL || keycode == KeyEvent.KEYCODE_BUTTON_THUMBR)
//    		{
//    			isAnalog = true;
//    		}
//    		
//    		if (sKeyMap.containsKey(keycode))
//    		{
//    			int controllerID = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
//    			Cocos2dxGameController.onButtonEvent(sVendorName, controllerID, sKeyMap.get(keycode), true, 1.0f, isAnalog);
//    		}
//        }
//
//        return handled; 
//    }
//
//    public static boolean onKeyUp(int keyCode, KeyEvent event) {
//        boolean handled = false;
//        handled = OuyaController.onKeyUp(keyCode, event);
//    
//        if (handled)
//        {
//    		int keycode = event.getKeyCode();
//    		boolean isAnalog = false;
//    		
//    		if (keycode == KeyEvent.KEYCODE_BUTTON_THUMBL || keycode == KeyEvent.KEYCODE_BUTTON_THUMBR)
//    		{
//    			isAnalog = true;
//    		}
//    		
//        	int controllerID = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
//        	Cocos2dxGameController.onButtonEvent(sVendorName, controllerID, sKeyMap.get(keycode), false, 0.0f, isAnalog);
//        }
//
//        return handled;
//    }
//
//    public static boolean onGenericMotionEvent(MotionEvent event) {
//        boolean handled = false;
//
//        handled = OuyaController.onGenericMotionEvent(event);
//
//        if (handled)
//        {
//        	OuyaController c = OuyaController.getControllerByDeviceId(event.getDeviceId());
//        	int controllerID = OuyaController.getPlayerNumByDeviceId(event.getDeviceId());
//        	
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.BUTTON_LEFT_TRIGGER, c.getAxisValue(OuyaController.AXIS_L2), true);
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.BUTTON_RIGHT_TRIGGER, c.getAxisValue(OuyaController.AXIS_R2), true);
//        	
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.LEFT_THUMBSTICK_X, c.getAxisValue(OuyaController.AXIS_LS_X), true);
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.LEFT_THUMBSTICK_Y, c.getAxisValue(OuyaController.AXIS_LS_Y), true);
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.RIGHT_THUMBSTICK_X, c.getAxisValue(OuyaController.AXIS_RS_X), true);
//        	Cocos2dxGameController.onAxisEvent(sVendorName, controllerID, Cocos2dxGameController.RIGHT_THUMBSTICK_Y, c.getAxisValue(OuyaController.AXIS_RS_Y), true);
//        }
//
//        return handled;
//    }
//    
//	public static void onPause() {
//        // show the mouse cursor
//        OuyaController.showCursor(true);
//	}
//	
//	public static void onResume() {
//        // hide the mouse cursor
//        OuyaController.showCursor(false);
//	}
//	
//	public static void onDestroy() {
//		
//	}
//}
