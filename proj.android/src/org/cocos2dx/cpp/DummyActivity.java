package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class DummyActivity extends Activity {
	@Override
    protected void onCreate(Bundle savedInstanceState) {       
        System.loadLibrary("cocos2dcpp");

        super.onCreate(savedInstanceState);

        Intent intent = new Intent(DummyActivity.this, org.cocos2dx.cpp.Cocos2dxActivity.class);
        DummyActivity.this.startActivity(intent);
    }
}
