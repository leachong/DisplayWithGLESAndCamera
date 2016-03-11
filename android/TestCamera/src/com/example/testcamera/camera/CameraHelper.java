package com.example.testcamera.camera;

import android.graphics.SurfaceTexture;
import android.os.Build;

public abstract class CameraHelper {
	
	public static CameraHelper Create() {
		return Create(CameraFacing.CameraFacingBack);
	}
	public static CameraHelper Create(CameraFacing facing) {
		CameraHelper ch;
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
        	ch = new CameraHelperOld();
        } else {
        	ch = new CameraHelperOld();
        }
    	ch.OpenCameraWithFacing(facing.value);
		return ch;
	}
	
	public abstract void StartPreview(SurfaceTexture surfaceTexture, int w, int h);
	public abstract int GetNumberOfCameras();
	public abstract boolean HasCamera(CameraFacing facing);
	public abstract Object GetCameraInstance();
	public abstract CameraFacing GetFacing();
	protected abstract void OpenCamera();
	protected abstract void OpenCamera(int id);
	protected abstract void OpenCameraWithFacing(int facing);

    public enum CameraFacing{
    	CameraFacingBack					(0),
    	CameraFacingFront					(1);
    	
    	private int value = 0;
        private CameraFacing(int value) 
        {
            this.value = value;
        }
        public int value() 
        {
            return this.value;
        }
        public static CameraFacing fromInt(int value)
        {
        	switch(value) {
        	case 0:
        		return CameraFacingBack;
        	case 1:
        		return CameraFacingFront;
        	}
        	return CameraFacingBack;
        }
    }
}
