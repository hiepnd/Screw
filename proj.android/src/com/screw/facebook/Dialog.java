package com.screw.facebook;

import java.util.Arrays;
import java.util.HashMap;
import java.util.UUID;

import org.json.JSONObject;

import android.os.Bundle;
import android.util.Log;

import com.facebook.model.GraphObject;
import com.facebook.model.OpenGraphAction;
import com.facebook.widget.*;
import com.facebook.widget.FacebookDialog.OpenGraphActionDialogBuilder;
import com.facebook.widget.FacebookDialog.OpenGraphActionDialogFeature;
import com.facebook.widget.FacebookDialog.PendingCall;
import com.facebook.widget.FacebookDialog.ShareDialogBuilder;
import com.facebook.widget.FacebookDialog.ShareDialogFeature;

public class Dialog {
	private static final int DEBUG = Facebook.DEBUG;
	private static final String TAG = "Screw.Dialog";
	private static final DialogCallback dialogCallback = new DialogCallback();
	private static final HashMap<UUID, Long> uuidMap = new HashMap<UUID, Long>();
	
	public static DialogCallback getDialogCallback() {
		return dialogCallback;
	}
	
	public static boolean canPresentShareDialog() {
		 return FacebookDialog.canPresentShareDialog(Facebook.getActivity(), ShareDialogFeature.SHARE_DIALOG);
	}
	
	public static boolean canSharePresentShareOpenGraphAction() {
		return FacebookDialog.canPresentOpenGraphActionDialog(Facebook.getActivity(), OpenGraphActionDialogFeature.OG_ACTION_DIALOG);
	}
	
	public static void presentShareDialog(final long requestCode, final Bundle data) {
		if (DEBUG > 0) {
			Log.d(TAG, "Present share dialog:\n{\n");
			Log.d(TAG, "	code = " + requestCode);
			Log.d(TAG, "	data = " + data);
			Log.d(TAG, "}");
		}
		Facebook.getActivity().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				ShareDialogBuilder builder = shareDialogBuilderFromBundle(data);
				PendingCall call = builder.build().present();
				uuidMap.put(call.getCallId(), requestCode);
				Facebook.getUiLifecycleHelper().trackPendingDialogCall(call);
			}
		});
	}
	
	public static void presentShareOpenGraphActionDialog(final long requestCode, final Bundle data) {
		if (DEBUG > 0) {
			Log.d(TAG, "Present share Open Graph Action:\n{\n");
			Log.d(TAG, "	code = " + requestCode);
			Log.d(TAG, "	data = " + data);
			Log.d(TAG, "}");
		}
			Facebook.getActivity().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				OpenGraphActionDialogBuilder builder = shareActionBuilderFromBundle(data);
				PendingCall call = builder.build().present();
				uuidMap.put(call.getCallId(), requestCode);
				Facebook.getUiLifecycleHelper().trackPendingDialogCall(call);
			}
		});
	}
	
	private static ShareDialogBuilder shareDialogBuilderFromBundle(Bundle data) {
		ShareDialogBuilder builder = new ShareDialogBuilder(Facebook.getActivity());
		
		//Link
		if (data.getString("link") != null) {
			builder.setLink(data.getString("link"));
		}
		
		//Name
		if (data.getString("name") != null) {
			builder.setName(data.getString("name"));
		}
		
		//Caption
		if (data.getString("caption") != null) {
			builder.setCaption(data.getString("caption"));
		}
		
		//Description
		if (data.getString("description") != null) {
			builder.setDescription(data.getString("description"));
		}
		
		//Picture
		if (data.getString("picture") != null) {
			builder.setPicture(data.getString("picture"));
		}
		
		//Ref
		if (data.getString("ref") != null) {
			builder.setRef(data.getString("ref"));
		}
		
		//DataFailuresFatal
		if (data.containsKey("dataFailuresFatal")) {
			builder.setDataErrorsFatal(Boolean.valueOf(data.getString("dataFailuresFatal")).booleanValue());
		}
		
		//Place
		if (data.getString("place") != null) {
			builder.setPlace(data.getString("place"));
		}
		
		//Friends
		if (data.getStringArray("friends") != null) {
			builder.setFriends(Arrays.asList(data.getStringArray("friends")));
		}
		
		return builder;
	}
	
	private static OpenGraphActionDialogBuilder shareActionBuilderFromBundle(Bundle data) {
		OpenGraphAction action = null;
		Bundle actionData = data.getBundle("action");
		if ( data != null) {
			try {
				JSONObject json = Helper.toJsonObject(actionData);
				action = GraphObject.Factory.create(json, OpenGraphAction.class);
			} catch (Exception e) {
				Log.d(TAG, "Error parse data " + Helper.toJsonString(actionData));
				e.printStackTrace();
			}
		}
		OpenGraphActionDialogBuilder builder = new OpenGraphActionDialogBuilder(Facebook.getActivity(), action, null);
		
		return builder;
	}
	
	public static class DialogCallback implements FacebookDialog.Callback {

		@Override
		public void onComplete(PendingCall pendingCall, Bundle data) {
			// TODO Auto-generated method stub
			if (DEBUG > 0) {
				Log.d(TAG, "Dialog callback complete:\n{\n");
				Log.d(TAG, "	uuid = " + pendingCall.getCallId());
				Log.d(TAG, "	data = " + data);
				Log.d(TAG, "}");
			}
			Long code = uuidMap.get(pendingCall.getCallId());
			if (code != null) {
				boolean didCancel = FacebookDialog.getNativeDialogDidComplete(data);
				String completionGesture = FacebookDialog.getNativeDialogCompletionGesture(data);
				String postId = FacebookDialog.getNativeDialogPostId(data);
				completeDialog(code, didCancel, completionGesture, postId, 0);
			}
		}

		@Override
		public void onError(PendingCall pendingCall, Exception error, Bundle data) {
			// TODO Auto-generated method stub
			if (DEBUG > 0) {
				Log.d(TAG, "Dialog callback error:\n{\n");
				Log.d(TAG, "	uuid = " + pendingCall.getCallId());
				Log.d(TAG, "	data = " + data);
				Log.d(TAG, "}");
			}
			Long code = uuidMap.get(pendingCall.getCallId());
			if (code != null) {
				boolean didCancel = FacebookDialog.getNativeDialogDidComplete(data);
				String completionGesture = FacebookDialog.getNativeDialogCompletionGesture(data);
				String postId = FacebookDialog.getNativeDialogPostId(data);
				completeDialog(code, didCancel, completionGesture, postId, 1);
			}
		}		
	}
	
	private static void completeDialog(final long code, final boolean didCancel, final String completionGesture, final String postId, final int error) {
		Facebook.getActivity().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				nativeCompleteDialog(code, didCancel, completionGesture != null ? completionGesture : "", 
									 postId != null ? postId : "", error);
			}
		});
	}
	private static native void nativeCompleteDialog(long code, boolean didCancel, String completionGesture, String postId, int error);
}
