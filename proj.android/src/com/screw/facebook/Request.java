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

import org.cocos2dx.lib.Cocos2dxHelper;

import com.facebook.HttpMethod;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.Request.Callback;

import android.os.Bundle;
import android.util.Log;

public class Request {
	private static final int DEBUG = com.screw.facebook.Session.DEBUG;
	private static final String TAG = "Screw.Request";
	
	public static void request(long requestCode, String graphPath,
			Bundle parameters, int method) {
		if (DEBUG > 0) {
			Log.d(TAG, "Executing request:\n{\n");
			Log.d(TAG, "	code = " + requestCode);
			Log.d(TAG, "	method = " + method);
			Log.d(TAG, "	graphPath = " + (graphPath == null ? "(null)" : graphPath));
			Log.d(TAG, "	params = " + (parameters == null ? "(null)" : parameters));
			Log.d(TAG, "}");
		}
		
		HttpMethod httpMethod = HttpMethod.GET;
		switch (method) {
		case 0:
			httpMethod = HttpMethod.GET;
			break;

		case 1:
			httpMethod = HttpMethod.POST;
			break;

		case 2:
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
				
				if (DEBUG > 0) {
					Log.d(TAG, "Request #" + requestCode_ + " completed\n{\n");
					
					if (response.getError() == null & DEBUG >= 2) {
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
	}

	static private native void nativeCallback(long requestCode, int errorCode, String errorMessage, String result);

}
