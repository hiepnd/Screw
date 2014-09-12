package com.screw.twitter;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;
import java.util.Locale;

import org.cocos2dx.lib.Cocos2dxHelper;

import com.screw.facebook.Facebook;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;

public class Twitter {
	public static final int TWEET_REQUEST_CODE = 8568;
	public static final String TAG = "";

	public static void tweet(final String message, final String imagePath) {
		Facebook.getActivity().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				tweetInternal(Facebook.getActivity(), message, imagePath);
			}
		});
	}

	/**
	 *
	 * @param activity
	 * @param message
	 * @param imagePath
	 * @warning Need android.permission.WRITE_EXTERNAL_STORAGE permission
	 */
	private static void tweetInternal(Activity activity, final String message, final String imagePath) {
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("image/*");
		intent.putExtra(android.content.Intent.EXTRA_TEXT, message);

		// Save image to external storage first
		File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
	    File file = new File(path, "ScrewTmp.png");

	    try {
	    	path.mkdirs();
	    	InputStream is;

			if (imagePath.startsWith("/assets/")) {
				// Assets file
				is = activity.getResources().getAssets().open(imagePath.substring(8));
			} else {
				is = new FileInputStream(imagePath);
			}
	    	OutputStream os = new FileOutputStream(file);

	    	byte[] data = new byte[is.available()];
	    	is.read(data);
	    	os.write(data);

	    	is.close();
	    	os.close();

	    } catch (IOException e) {
	    	e.printStackTrace();
	    }

	    // Image
	    if (file.exists()) {
		    Uri uri = Uri.fromFile(file);
			intent.putExtra(Intent.EXTRA_STREAM, uri);
	    }
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
