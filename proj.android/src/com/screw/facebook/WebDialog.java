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

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.cocos2dx.lib.Cocos2dxHelper;

import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.internal.ServerProtocol;

import android.os.Bundle;
import android.util.Log;

public class WebDialog {
	private static final int DEBUG = Session.DEBUG;
	private static final String TAG = "Screw.WebDialog";
	
	public static void show(final long requestCode, final String action, final Bundle params) {
		if (DEBUG > 0) {
			Log.d(TAG, "Showing dialog:\n{\n");
			Log.d(TAG, "	requestCode = " + requestCode);
			Log.d(TAG, "	action = " + (action == null ? "(null)" : action));
			Log.d(TAG, "	params = " + (params == null ? "(null)" : params));
			Log.d(TAG, "}");
		}
		params.putString("frictionless", "1");
		
		final com.facebook.widget.WebDialog.OnCompleteListener callback = new com.facebook.widget.WebDialog.OnCompleteListener() {
			@Override
			public void onComplete(Bundle values, FacebookException error) {
				// TODO Auto-generated method stub
				int errorCode = 0;
				final String json = Helper.toJsonString(values);
				final String errorMessage = "";
				if (error != null) {
					errorCode = 1;
					errorMessage.concat("Something wrong !!! - Enable DEBUG in WebDialog for detail");
					if (error instanceof FacebookOperationCanceledException) {
						errorMessage.concat("Request cancelled");
					}
				}
				if (DEBUG > 0) {
					Log.d(TAG, "Request #" + requestCode + " completed\n{\n");
					Log.d(TAG, "	Values = " + values);
					Log.d(TAG, "	Json = " + json);
					if (errorCode != 0) {
						Log.d(TAG, "	Error = " + error);
					}
					Log.d(TAG, "}");
				}
				
				final int errorCode_ = errorCode; 
				Cocos2dxHelper.runOnGLThread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						nativeCompleteAppRequest(requestCode, errorCode_, errorMessage, json);
					}
				});
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
		
		Session.getActivity().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				com.facebook.widget.WebDialog dialog = new com.facebook.widget.WebDialog(Session.getActivity(), action, params, com.facebook.widget.WebDialog.DEFAULT_THEME, callback);
				dialog.show();
			}
		});
	}
	
	private static native void nativeCompleteAppRequest(long requestCode, int error, String errorMessage, String jsonResponse);
}
