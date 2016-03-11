package com.example.testcamera;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        GLSurfaceViewCus glView = (GLSurfaceViewCus)findViewById(R.id.gLSurfaceViewCus1);
    }
}
