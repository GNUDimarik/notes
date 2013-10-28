package com.packtpub;

import android.app.Activity;
import android.os.Bundle;

public class LiveCameraActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new CameraView(this));
    }
}