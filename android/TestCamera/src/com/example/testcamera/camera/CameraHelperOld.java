package com.example.testcamera.camera;

import java.io.IOException;
import java.util.List;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.util.Log;

@SuppressWarnings("deprecation")
class CameraHelperOld extends CameraHelper {

	private int _cameraID;
	private Camera _camera;

	@Override
	public void StartPreview(SurfaceTexture surfaceTexture, int w, int h) {
		if(surfaceTexture==null)
			return;
		if(_camera==null)
			this.OpenCamera();

        try {        
        	Camera.Parameters parameters = _camera.getParameters();
//        	List<Size> lst = parameters.getSupportedPreviewSizes();
//        	for(int i=0; i<lst.size();i++){
//        		Log.v(_logTag,String.format("onSurfaceChanged the supported size:%d,%d",lst.get(i).width,lst.get(i).height));
//        	}
//        	List<Integer> lst2 = parameters.getSupportedPreviewFrameRates();
//        	for(int i=0; i<lst2.size();i++){
//        		Log.v(_logTag,String.format("onSurfaceChanged the supported rate:%d",lst2.get(i)));
//        	}
//        	parameters.setPreviewFrameRate(30);

            if (parameters.getSupportedFocusModes().contains(
                    Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
                parameters.setFocusMode(Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
            }
        	parameters.setPreviewSize(w,h);
        	_camera.setParameters(parameters);
        	_camera.setPreviewTexture(surfaceTexture);
        	_camera.startPreview();
        	
        } catch (IOException e) {
            e.printStackTrace();
        }
	}
	@Override
	public int GetNumberOfCameras() {
        return Camera.getNumberOfCameras();
	}

	@Override
	public boolean HasCamera(CameraFacing facing) {
		return getCameraId(facing.value()) != -1;
	}

	@Override
	protected void OpenCamera() {
		this.OpenCamera(0);
	}
	
	@Override
	protected void OpenCamera(int id) {
		this.releaseCamera();
		_cameraID = id;
		if(_cameraID < 0)
			_cameraID = 0;
		_camera = Camera.open(_cameraID);
	}

	@Override
	protected void OpenCameraWithFacing(int facing) {
		this.OpenCamera(getCameraId(facing));
	}

	@Override
	public Object GetCameraInstance() {
		return _camera;
	}

    private int getCameraId(final int facing) {
        int numberOfCameras = Camera.getNumberOfCameras();
        CameraInfo info = new CameraInfo();
        for (int id = 0; id < numberOfCameras; id++) {
            Camera.getCameraInfo(id, info);
            if (info.facing == facing) {
                return id;
            }
        }
        return -1;
    }
    private void releaseCamera() {
    	if(_camera != null) {
        	_camera.setPreviewCallback(null);
        	_camera.release();
        	_camera = null;
    	}
    }
	@Override
	public CameraFacing GetFacing() {
		CameraInfo info = new CameraInfo();
		Camera.getCameraInfo(_cameraID, info);
		return CameraFacing.fromInt(info.facing);
	}
}
