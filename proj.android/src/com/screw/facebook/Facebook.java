package com.screw.facebook;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.facebook.UiLifecycleHelper;

public class Facebook {
	/* Debug: 0 - disabled, 1 - info, 2 - data */
	public static final int DEBUG = 2;
	public static final String TAG = "Screw.Facebook";
	
	private static UiLifecycleHelper lifecycleHelper;
	private static Activity activity = null;
	
	public static Activity getActivity() {
		return activity;
	}
	
	public static UiLifecycleHelper getUiLifecycleHelper() {
		return lifecycleHelper;
	}
	
	public static void onActivityCreate(Activity activity, Bundle savedInstanceState) {
		Facebook.activity = activity;
		Session.onActivityCreate(activity, savedInstanceState);
		lifecycleHelper = new UiLifecycleHelper(activity, Session.getStatusCallback());
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (DEBUG > 0)
			Log.d(TAG, "onActivityResult");
		lifecycleHelper.onActivityResult(requestCode, resultCode, data, Dialog.getDialogCallback());
	}
	
	public static void onActivityResume() {

	}

	public static void onActivityPause() {
		
	}

	public static void onActivityDestroy() {
		Session.onActivityDestroy();
		activity = null;
	}
	
	
}
