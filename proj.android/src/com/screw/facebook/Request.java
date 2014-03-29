package com.screw.facebook;

import org.cocos2dx.lib.Cocos2dxHelper;

import com.facebook.HttpMethod;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.Request.Callback;

import android.os.Bundle;
import android.util.Log;

public class Request {
	private static final boolean DEBUG = com.screw.facebook.Session.DEBUG && false;
	private static final String TAG = "Screw.Request";
	
	public static void request(long requestCode, String graphPath,
			Bundle parameters, int method) {
		if (DEBUG) {
			Log.d(TAG, "request:\n{\n");
			Log.d(TAG, "	code = " + requestCode);
			Log.d(TAG, "	method = " + method);
			Log.d(TAG, "	graphPath = " + (graphPath == null ? "(null)" : graphPath));
			Log.d(TAG, "	params = " + (parameters == null ? "(null)" : parameters));
			Log.d(TAG, "}");
		}
		
		HttpMethod httpMethod = HttpMethod.GET;
		switch (method) {
		case 1:
			httpMethod = HttpMethod.GET;
			break;

		case 2:
			httpMethod = HttpMethod.POST;
			break;

		case 3:
			httpMethod = HttpMethod.DELETE;
			break;
		}
		final long requestCode_ = requestCode;
		final com.facebook.Request request = new com.facebook.Request(com.facebook.Session.getActiveSession(),
				graphPath, parameters, httpMethod, null);
		Callback callback = new Callback() {

			@Override
			public void onCompleted(Response response) {
				// TODO Auto-generated method stub
				final int errorCode = response.getError() == null ? 0 : response.getError().getErrorCode();
				final String errorMessage = response.getError() == null ? "" : response.getError().getErrorMessage();
				final String responseText = response.getGraphObject() == null ? "" : response.getGraphObject().getInnerJSONObject().toString();
				
				if (DEBUG) {
					Log.d(TAG, "Request #" + requestCode_ + " completed\n{\n");
					
					if (response.getError() == null) {
						Log.d(TAG, "	Data = " + responseText);
					} else {
						Log.d(TAG, "	Error error: " + errorMessage);
					}
					Log.d(TAG, "}");
				}
				Cocos2dxHelper.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						nativeCallback(requestCode_, errorCode, errorMessage, responseText);
					}
				});
			}
		};
		request.setCallback(callback);
		
		com.screw.facebook.Session.getHandler().post(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				request.executeAsync();
			}
		});
		
		if (DEBUG) {
			Log.d(TAG, "Executing request #" + requestCode);
		}
	}

	static private native void nativeCallback(long requestCode, int errorCode, String errorMessage, String result);
	
	static public void test(Bundle params) {
		Log.d("HHHhhhhhhhhhhHHHHHHHH", "hhhhh");
		for (String key : params.keySet()) {
			Object val = params.get(key);
			if (val instanceof String) {
				Log.d("Request Test", key + " = " + (String) val);
			}

			if (val instanceof String[]) {
				String[] list = (String[]) val;
				String str = "[";
				for (String s : list) {
					str += s + " ";
				}
				str += "]";
				Log.d("Request Test", key + " = " + str);
			}

			if (val instanceof Bundle) {
				test((Bundle) val);
			}
		}
	}
}
