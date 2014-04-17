/****************************************************************************
 Copyright (c) hiepndhut@gmail.com
 Copyright (c) 2014 No PowerUp Games
 
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

package com.screw.facebook;

import java.util.Arrays;
import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.facebook.*;
import com.facebook.Session.NewPermissionsRequest;
import com.facebook.Session.OpenRequest;

public class Session {
	/* Debug: 0 - disabled, 1 - info, 2 - data */
	public static final int DEBUG = 2;
	public static final String TAG = "Screw.Session";

	private static boolean _started = false;
	private static Activity _activity = null;
	private static SessionStatusCallback _statusCallback = new SessionStatusCallback();

	public static void onActivityCreate(Activity activity, Bundle savedInstanceState) {
		_activity = activity;
		
		com.facebook.Session session = com.facebook.Session.getActiveSession();
		if (session == null) {
			if (savedInstanceState != null) {
				session = com.facebook.Session.restoreSession(_activity, null, _statusCallback, savedInstanceState);
			}
			if (session == null) {
				session = new com.facebook.Session(_activity);
			}
		}
		com.facebook.Session.setActiveSession(session);
	}
	
	/*  Get called from native code - applicationDidFinishLaunching */
	static public void start() {
		assert !_started : "Facebook must be start only once";
		_started = true;
		
		com.facebook.Session session = com.facebook.Session.getActiveSession();
		assert session != null : "Session must not be null";
		session.addCallback(_statusCallback);
		String[] permissions = new String[session.getPermissions().size()];
		if (DEBUG > 0) {
			Log.d(TAG, "Starting with appid = " + session.getApplicationId());
		}
		nativeInit(getNativeState(session.getState()), session.getApplicationId(), 
					session.getPermissions().toArray(permissions));
	}

	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (DEBUG > 0)
			Log.d(TAG, "onActivityResult");
		com.facebook.Session.getActiveSession().onActivityResult(_activity, requestCode, resultCode, data);
	}

	public static void onActivityResume() {

	}

	public static void onActivityPause() {

	}

	public static void onActivityDestroy() {
		_activity = null;
	}

	public static void open(final boolean allowUi, final String[] permissions, final int defaultAudience, final int loginBehavior) {
		if (DEBUG > 0) {
			Log.d(TAG, "Open session:\n{\n");
			Log.d(TAG, "	allowUI = " + allowUi);
			Log.d(TAG, "	permissions = " + Arrays.asList(permissions));
			Log.d(TAG, "	defaultAudience = " + getDefaultAudience(defaultAudience));
			Log.d(TAG, "	loginBehavior = " + getLoginBehavior(loginBehavior));
			Log.d(TAG, "}");
		}
		_activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (com.facebook.Session.getActiveSession().isClosed()) {
					com.facebook.Session.getActiveSession().removeCallback(_statusCallback);
					com.facebook.Session session = new com.facebook.Session(_activity);
					session.addCallback(_statusCallback);
					com.facebook.Session.setActiveSession(session);
				}

				com.facebook.Session session = com.facebook.Session.getActiveSession();
				OpenRequest request = new OpenRequest(_activity);
				request.setLoginBehavior(getLoginBehavior(loginBehavior));
				request.setDefaultAudience(getDefaultAudience(defaultAudience));
				request.setPermissions(Arrays.asList(permissions));
				if (SessionState.CREATED_TOKEN_LOADED.equals(session.getState()) || allowUi) {
					session.openForRead(request);
				}
			}
		});
	}

	public static void close() {
		_activity.runOnUiThread(new Runnable() {
			
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

	public static void requestReadPermissions(String[] permissions) {
		if (DEBUG > 0) {
			Log.d(TAG, "Request read permissions:\n{\n");
			Log.d(TAG, "	permissions = " + Arrays.asList(permissions));
			Log.d(TAG, "}");
		}
		final NewPermissionsRequest request = new NewPermissionsRequest(_activity, Arrays.asList(permissions));
		_activity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				com.facebook.Session.getActiveSession().requestNewReadPermissions(request);
			}
		});
	}

	public static void requestPublishPermissions(String[] permissions) {
		if (DEBUG > 0) {
			Log.d(TAG, "Request publish permissions:\n{\n");
			Log.d(TAG, "	permissions = " + Arrays.asList(permissions));
			Log.d(TAG, "}");
		}
		final NewPermissionsRequest request = new NewPermissionsRequest(_activity, Arrays.asList(permissions));
		_activity.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				com.facebook.Session.getActiveSession().requestNewPublishPermissions(request);
			}
		});
	}

	public static Activity getActivity() {
		return _activity;
	}

	static public class SessionStatusCallback implements com.facebook.Session.StatusCallback {

		@Override
		public void call(com.facebook.Session session, SessionState state, Exception exception) {
			// TODO Auto-generated method stub
			if (DEBUG > 0) {
				Log.d(TAG, "SessionStatusCallback\n{\n");
				Log.d(TAG, "	State = " + state);
				if (exception != null) {
					Log.d(TAG, "	Exception = " + exception);
				}
				Log.d(TAG, "}");
			}
			final int state_ = getNativeState(state);
			final com.facebook.Session session_ = session;
			Cocos2dxHelper.runOnGLThread(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					String[] permissions = new String[session_.getPermissions().size()];
					nativeUpdateState(state_, session_.getPermissions().toArray(permissions));
				}
			});
		}
	}

	/* JNI */
	static private native void nativeInit(int state, String appid, String[] permissions);
	static private native void nativeUpdateState(int state, String[] permissions);

	static private int getNativeState(SessionState state) {
		int code = 0;
		switch (state) {
		case CREATED:
			code = 1;
			break;

		case CREATED_TOKEN_LOADED:
			code = 2;
			break;

		case OPENING:
			code = 3;
			break;

		case OPENED:
			code = 4;
			break;

		case OPENED_TOKEN_UPDATED:
			code = 5;
			break;

		case CLOSED_LOGIN_FAILED:
			code = 6;
			break;

		case CLOSED:
			code = 7;
			break;

		default:
			break;
		}

		return code;
	}
	
	private static SessionLoginBehavior getLoginBehavior(int behavior) {
		switch (behavior) {
			case 0: //WITH_FALLBACK_TO_WEBVIEW
				return SessionLoginBehavior.SSO_WITH_FALLBACK;
			case 1: //WITH_NO_FALLBACK_TO_WEBVIEW
				return SessionLoginBehavior.SSO_ONLY;
			case 2: //FORCE_WEBVIEW
				return SessionLoginBehavior.SUPPRESS_SSO;
			case 3: //SYSTEM_IF_PRESENT
				return SessionLoginBehavior.SSO_WITH_FALLBACK;
		}
		
		assert false : "Unknown behavior";
		return SessionLoginBehavior.SSO_WITH_FALLBACK;
	}
	
	private static SessionDefaultAudience getDefaultAudience(int audience) {
		switch (audience) {
			case 0: //NONE
				return SessionDefaultAudience.NONE;
			case 1: //ONLY_ME
				return SessionDefaultAudience.ONLY_ME;
			case 2: //FRIENDS
				return SessionDefaultAudience.FRIENDS;
			case 3: //PUBLIC
				return SessionDefaultAudience.EVERYONE;
				
			default:
				break;
		}
		return SessionDefaultAudience.NONE;
	}
}