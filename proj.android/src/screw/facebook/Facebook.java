package screw.facebook;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.facebook.*;

public class Facebook  {
	private static boolean _started = false;
	private static Activity _activity = null;
	private static SessionStatusCallback _statusCallback = new SessionStatusCallback();
	
	public static void start(Activity activity, Bundle savedInstanceState) {
		assert _started = false : "Facebook must be start only once";
		_started = true;
		_activity = activity;
		
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
            /** Open session if it's already loaded 
            if (session.getState().equals(SessionState.CREATED_TOKEN_LOADED)) {
                session.openForRead(new Session.OpenRequest(_activity).setCallback(_statusCallback));
            }
            */
            Log.d("Facebook", "Initializing");
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
	
	static public class SessionStatusCallback implements Session.StatusCallback {

		@Override
		public void call(Session session, SessionState state,
				Exception exception) {
			// TODO Auto-generated method stub
			nativeUpdateSessionState(getNativeState(state));
		}
	}
	
	/* JNI */
	static private native void nativeInitSession(int state, String appid);
	static private native void nativeUpdateSessionState(int state);
	static public native void nativeFoo(int bar);
	
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
