package com.example.testcamera.jni;

public class TestCameraLib {
	static {
        System.loadLibrary("TestCamera");
	}
	
    public static native void Init();
    public static native void Draw();
    public static native void SetTexture(int texid);
    public static native void SetInputSize(int width, int height);
    public static native void SetOutputSize(int width, int height);
    public static native void SetRotationMode(int rotationMode);
}
