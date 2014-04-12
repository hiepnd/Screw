package org.cocos2dx.cpp;

import com.screw.facebook.*;

import android.app.NativeActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;


public class Cocos2dxActivity extends NativeActivity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		System.loadLibrary("cocos2dcpp");
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		//For supports translucency
		
		//1.change "attribs" in cocos\2d\platform\android\nativeactivity.cpp
		/*const EGLint attribs[] = {
	            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  
	            //EGL_BLUE_SIZE, 5,   -->delete 
	            //EGL_GREEN_SIZE, 6,  -->delete 
	            //EGL_RED_SIZE, 5,    -->delete 
	            EGL_BUFFER_SIZE, 32,  //-->new field
	            EGL_DEPTH_SIZE, 16,
	            EGL_STENCIL_SIZE, 8,
	            EGL_NONE
	    };*/
		
		//2.Set the format of window
		// getWindow().setFormat(PixelFormat.TRANSLUCENT);
		
		Log.d("Cocos2dxActivity", "onCreate");
		Session.onActivityCreate(this, savedInstanceState);
		
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Session.onActivityPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		Session.onActivityResume();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		Session.onActivityResult(requestCode, resultCode, data);
	}
}
