package screw.facebook;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.facebook.*;

public class Facebook  {
	private static boolean _started = false;
	private static Activity _activity = null;
	private static SessionStatusCallback _statusCallback = new SessionStatusCallback();
	private static Handler _handler;
	
	public static void start(Activity activity, Bundle savedInstanceState) {
		assert _started = false : "Facebook must be start only once";
		_started = true;
		_activity = activity;
		_handler = new Handler();
		
		//Create Session Session
		Session session = Session.getActiveSession();
        if (session == null) {
            if (savedInstanceState != null) {
                session = Session.restoreSession(_activity, null, _statusCallback, savedInstanceState);
            }
            if (session == null) {
                session = new Session(_activity);
            }
            Session.setActiveSession(session);
            nativeInitSession(getNativeState(session.getState()), session.getApplicationId());
        }
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		Session.getActiveSession().onActivityResult(_activity, requestCode, resultCode, data);
	}
	
	public static void onApplicationResume() {
		
	}
	
	public static void onApplicationPause() {
		
	}
	
	public static void onApplicationDestroy() {
		_activity = null;
	}
	
	public static void openSession() {
		_handler.post(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Session.OpenRequest request = new Session.OpenRequest(_activity);
				request.setCallback(_statusCallback);
				Session.getActiveSession().openForRead(request);
			}
		});
	}
	
	public static void closeSession() {
		_handler.post(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Session.getActiveSession().closeAndClearTokenInformation();
				Session session = new Session(_activity);
				Session.setActiveSession(session);
			}
		});
	}
	
	static public class SessionStatusCallback implements Session.StatusCallback {

		@Override
		public void call(Session session, SessionState state,
				Exception exception) {
			// TODO Auto-generated method stub
			final SessionState _state = state;
			Cocos2dxHelper.runOnGLThread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					nativeUpdateSessionState(getNativeState(_state));
				}
			});
		}
	}
	
	/* JNI */
	static private native void nativeInitSession(int state, String appid);
	static private native void nativeUpdateSessionState(int state);
	
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
