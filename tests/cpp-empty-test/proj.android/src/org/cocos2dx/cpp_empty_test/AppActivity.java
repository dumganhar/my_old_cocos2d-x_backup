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

import org.cocos2dx.lib.Cocos2dxActivity;

import com.nibiru.lib.controller.Controller;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class AppActivity extends Cocos2dxActivity {
	
//	private Cocos2dxControllerMoga mMogaController = new Cocos2dxControllerMoga();
	private Cocos2dxControllerNibiru mNibiruController = new Cocos2dxControllerNibiru();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
//		mMogaController.onCreate(this);
		mNibiruController.onCreate(this);
//		Cocos2dxOuyaController.onCreate(this);
		
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
//		mMogaController.onPause();
		mNibiruController.onPause();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
//		mMogaController.onResume();
		mNibiruController.onResume();
	}
	
	@Override
	protected void onDestroy() {
//		mMogaController.onDestroy();
		mNibiruController.onDestroy();
		super.onDestroy();
	}
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        boolean handled = false;
//        handled = Cocos2dxOuyaController.onKeyDown(keyCode, event);

        handled = mNibiruController.onKeyDown(keyCode, event);
        
        return handled || super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        boolean handled = false;
//        handled = Cocos2dxOuyaController.onKeyUp(keyCode, event);
    
        handled = mNibiruController.onKeyUp(keyCode, event);
        
        return handled || super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        boolean handled = false;
//        handled = Cocos2dxOuyaController.onGenericMotionEvent(event);

        handled = mNibiruController.onGenericMotionEvent(event);
        
        return handled || super.onGenericMotionEvent(event);
    }
}
