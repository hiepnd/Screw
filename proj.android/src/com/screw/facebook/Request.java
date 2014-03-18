package com.screw.facebook;

import android.os.Bundle;
import android.util.Log;

public class Request {

	static public void test(Bundle params) {
		for (String key : params.keySet()) {
			Log.d("Request Test", key + " = " + params.getString(key));
		}
	}
}
