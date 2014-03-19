package com.screw.facebook;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.facebook.*;
import com.facebook.Session.NewPermissionsRequest;

public class Session  {
	private static boolean _started = false;
	private static Activity _activity = null;
	private static SessionStatusCallback _statusCallback = new SessionStatusCallback();
	private static Handler _handler;
	
	public static Handler getHandler() {
		return _handler;
	}
	
	public static void start(Activity activity, Bundle savedInstanceState) {
		assert _started = false : "Facebook must be start only once";
		_started = true;
		_activity = activity;
		_handler = new Handler();
		
		//Create Session Session
		com.facebook.Session session = com.facebook.Session.getActiveSession();
        if (session == null) {
            if (savedInstanceState != null) {
                session = com.facebook.Session.restoreSession(_activity, null, _statusCallback, savedInstanceState);
            }
            if (session == null) {
                session = new com.facebook.Session(_activity);
            }
            com.facebook.Session.setActiveSession(session);
            session.addCallback(_statusCallback);
            final String[] permissions = new String[session.getPermissions().size()];
            final com.facebook.Session session_ = session;
            nativeInit(getNativeState(session_.getState()), session_.getApplicationId(), session_.getPermissions().toArray(permissions));
//            Cocos2dxHelper.runOnGLThread(new Runnable() {
//				
//				@Override
//				public void run() {
//					// TODO Auto-generated method stub
//					nativeInit(getNativeState(session_.getState()), session_.getApplicationId(), session_.getPermissions().toArray(permissions));
//				}
//			});
        }
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		com.facebook.Session.getActiveSession().onActivityResult(_activity, requestCode, resultCode, data);
	}
	
	public static void onApplicationResume() {
		
	}
	
	public static void onApplicationPause() {
		
	}
	
	public static void onApplicationDestroy() {
		_activity = null;
		_handler = null;
	}
	
	public static void open() {
		_handler.post(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				com.facebook.Session.OpenRequest request = new com.facebook.Session.OpenRequest(_activity);
				com.facebook.Session.getActiveSession().openForRead(request);
			}
		});
	}
	
	public static void close() {
		_handler.post(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				com.facebook.Session.getActiveSession().closeAndClearTokenInformation();
				com.facebook.Session session = new com.facebook.Session(_activity);
				session.addCallback(_statusCallback);
				com.facebook.Session.setActiveSession(session);
			}
		});
	}
	
	public static void requestReadPermissions(String[] permission) {
		NewPermissionsRequest request = new NewPermissionsRequest(_activity, Arrays.asList(permission));
		com.facebook.Session.getActiveSession().requestNewReadPermissions(request);
	}
	
	public static void requestPublishPermissions(String[] permission) {
		NewPermissionsRequest request = new NewPermissionsRequest(_activity, Arrays.asList(permission));
		com.facebook.Session.getActiveSession().requestNewPublishPermissions(request);
	}
	
	static public class SessionStatusCallback implements com.facebook.Session.StatusCallback {

		@Override
		public void call(com.facebook.Session session, SessionState state,
				Exception exception) {
			// TODO Auto-generated method stub
			final int state_ = getNativeState(state);
			final com.facebook.Session session_ = session;
			Cocos2dxHelper.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					String[] permissions = new String[session_.getPermissions().size()];
					nativeUpdateState(state_, session_.getPermissions().toArray(permissions));
					
					//if login failed - recreate session for subsequent requests
					if (session_.getState() == SessionState.CLOSED_LOGIN_FAILED) {
						com.facebook.Session session = new com.facebook.Session(_activity);
						com.facebook.Session.setActiveSession(session);
						session.addCallback(_statusCallback);
					}
				}
			});
		}
	}
	
	/* JNI */
	static private native void nativeInit(int state, String appid, String[] permissions);
	static private native void nativeUpdateState(int state, String[] permissions);
	
	static private int getNativeState(SessionState state) {
		int code = -1;
		switch (state) {
		case CREATED:
			code = 0;
			break;
			
		case CREATED_TOKEN_LOADED:
			code = 1;
			break;
			
		case OPENING:
			code = 2;
			break;
			
		case OPENED:
			code = 3;
			break;

		case OPENED_TOKEN_UPDATED:
			code = 4;
			break;
			
		case CLOSED_LOGIN_FAILED:
			code = 5;
			break;

		case CLOSED:
			code = 6;
			break;

		default:
			break;
		}
		
		return code;
	}
}
