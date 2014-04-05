package com.screw.facebook;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.internal.ServerProtocol;
import com.facebook.widget.WebDialog;

import android.os.Bundle;
import android.util.Log;

public class Dialog {
	private static final int DEBUG = Session.DEBUG;
	private static final String TAG = "Screw.Dialog";
	
	public static void show(final long requestCode, final String action, final Bundle params) {
		if (DEBUG > 0) {
			Log.d(TAG, "Showing dialog:\n{\n");
			Log.d(TAG, "	requestCode = " + requestCode);
			Log.d(TAG, "	action = " + (action == null ? "(null)" : action));
			Log.d(TAG, "	params = " + (params == null ? "(null)" : params));
			Log.d(TAG, "}");
		}
		params.putString("frictionless", "1");
		
		final WebDialog.OnCompleteListener callback = new WebDialog.OnCompleteListener() {
			@Override
			public void onComplete(Bundle values, FacebookException error) {
				// TODO Auto-generated method stub
				int errorCode = 0;
				String errorMessage = "";
				String requestId = "";
				List<String> toes = new ArrayList<String>();
				if (error != null) {
					errorCode = 1;
					errorMessage = "Something wrong !!! - Enable DEBUG in Dialog for detail";
					if (error instanceof FacebookOperationCanceledException) {
						errorMessage = "Request cancelled";
					}
				} else {
					//Don't send null to native code
					requestId = values.getString("request") != null ? values.getString("request") : "";
					int i = 0;
					while (true) {
						String to = values.getString("to[" + i + "]") ;
						if (to != null) {
							toes.add(to);
						} else {
							break;
						}
						i++;
					}
				}
				if (DEBUG > 0) {
					Log.d(TAG, "Request #" + requestCode + " completed\n{\n");
					if (errorCode != 0) {
						Log.d(TAG, "	Error = " + error);
					} else {
						Log.d(TAG, "	RequestId = '" + requestId + "'");
						Log.d(TAG, "	Receivers = " + toes + "");
					}
					Log.d(TAG, "}");
				}
				nativeCompleteAppRequest(requestCode, errorCode, errorMessage, requestId, toes.toArray(new String[toes.size()]));
			}
		};
		
		/* Refer to Buillder.build() */
		com.facebook.Session session = com.facebook.Session.getActiveSession();
		if (session != null && session.isOpened()) {
			params.putString(ServerProtocol.DIALOG_PARAM_APP_ID, session.getApplicationId());
			params.putString(ServerProtocol.DIALOG_PARAM_ACCESS_TOKEN, session.getAccessToken());
        } else {
        	params.putString(ServerProtocol.DIALOG_PARAM_APP_ID, session.getApplicationId());
        }
		
		Session.getHandler().post(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				WebDialog dialog = new WebDialog(Session.getActivity(), action, params, WebDialog.DEFAULT_THEME, callback);
				dialog.show();
			}
		});
	}
	
	private static native void nativeCompleteAppRequest(long requestCode, int error, String errorMessage, String requestId, String []toes);
}
