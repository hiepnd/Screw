package com.screw.twitter;

import java.util.List;
import java.util.Locale;

import org.cocos2dx.lib.Cocos2dxHelper;

import com.screw.facebook.Facebook;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.util.Log;

public class Twitter {
	public static final int TWEET_REQUEST_CODE = 8568;
	public static final String TAG = "";
	
	public static void tweet(final String message) {
		Facebook.getActivity().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				tweetInternal(Facebook.getActivity(), message);
			}
		});
	}
	
	private static void tweetInternal(Activity activity, final String message) {
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("text/plain");
		intent.putExtra(android.content.Intent.EXTRA_TEXT, message); 
			
		// Narrow down to official Twitter app, if available:
		boolean found = false;
		List<ResolveInfo> matches = activity.getPackageManager().queryIntentActivities(intent, 0);
		if (matches.size() == 0) {
			callback(2);
			return;
		}
		
		for (ResolveInfo info : matches) {
		    if (info.activityInfo.packageName.toLowerCase(Locale.US).startsWith("com.twitter")) {
		        intent.setPackage(info.activityInfo.packageName);
		        found = true;
		        break;
		    }
		}

		if (found) {
			activity.startActivityForResult(intent, TWEET_REQUEST_CODE);
		} else {
			activity.startActivityForResult(Intent.createChooser(intent, ""), TWEET_REQUEST_CODE);
		}
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == TWEET_REQUEST_CODE) {
			if (resultCode == Activity.RESULT_OK) {
				callback(0);
			} else {
				Log.d(TAG, "Tweet CANCEL");
				callback(1);
			}
		}
	}
	
	private static void callback(final int error) {
		Cocos2dxHelper.runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				nativeCallback(error);
			}
		});
	}
	
	private native static void nativeCallback(int error);
}
