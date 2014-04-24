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

import java.lang.reflect.Array;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Bundle;
import android.util.Log;

public class Helper {
	public static String toJsonString(Bundle bundle) {
		return bundle == null ? "{}" : toJsonObject(bundle).toString();	
	}
	
	public static JSONObject toJsonObject(Bundle bundle) {
		JSONObject jsonObject = new JSONObject();
		try {
			for (String key : bundle.keySet()) {
				Object elm = bundle.get(key);
				if (elm instanceof Bundle) {
					jsonObject.put(key, toJsonObject((Bundle) elm));
				} else if (elm instanceof List<?>) {
					jsonObject.put(key, toJsonArray((List<?>) elm));
				} else if (elm instanceof Array) {
					jsonObject.put(key, toJsonArray((Array) elm));
				} else {
					jsonObject.put(key, elm.toString());
				}
			}
		} catch (JSONException e) {
			Log.d("Helper.toJsonObject", "Exception");
			e.printStackTrace();
		}
		
		return jsonObject;
	}
	
	public static JSONArray toJsonArray(List<?> array) {
		JSONArray jsonArray = new JSONArray();
		for (Object elm : array) {
			if (elm instanceof Bundle) {
				jsonArray.put(toJsonObject((Bundle) elm));
			} else if (elm instanceof List<?>) {
				jsonArray.put(toJsonArray((List<?>) elm));
			} else if (elm instanceof Array) {
				jsonArray.put(toJsonArray((Array) elm));
			} else {
				jsonArray.put(elm.toString());
			}
		}
		
		return jsonArray;
	}
	
	public static JSONArray toJsonArray(Array array) {
		JSONArray jsonArray = new JSONArray();
		for (int i = 0; i < Array.getLength(array); i++) {
			Object elm = Array.get(array, i);
			if (elm instanceof Bundle) {
				jsonArray.put(toJsonObject((Bundle) elm));
			} else if (elm instanceof List<?>) {
				jsonArray.put(toJsonArray((List<?>) elm));
			} else if (elm instanceof Array) {
				jsonArray.put(toJsonArray((Array) elm));
			} else {
				jsonArray.put(elm.toString());
			}
		}
		return jsonArray;
	}
}
