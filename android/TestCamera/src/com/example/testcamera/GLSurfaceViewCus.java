package com.example.testcamera;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.example.testcamera.camera.CameraHelper;
import com.example.testcamera.gles.GLESHelper;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class GLSurfaceViewCus extends GLSurfaceView implements OnFrameAvailableListener{

	private GLESHelper _glesHelper; 
	private CameraHelper _camera;
    public GLSurfaceViewCus(Context context) {
        super(context);
        init();
    }
    public GLSurfaceViewCus(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
	private void init() {
		
        setEGLContextClientVersion(2);
        setRenderer(new RendererCus());
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        
        _camera = CameraHelper.Create(CameraHelper.CameraFacing.CameraFacingFront);
	}
	
	@Override
	public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        this.queueEvent(new Runnable(){
        	@Override
            public void run() {
        		GLSurfaceViewCus.this.requestRender();
            }
        });
	}
	
	private class RendererCus implements GLSurfaceView.Renderer {

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			GLSurfaceViewCus.this.queueEvent(new Runnable(){
            	@Override
                public void run() {
            		_glesHelper = new GLESHelper(GLSurfaceViewCus.this);
                }
            });
		}

		@Override
		public void onSurfaceChanged(GL10 gl, final int width, final int height) {
			GLSurfaceViewCus.this.queueEvent(new Runnable(){
            	@Override
                public void run() {
                    if(_camera.GetFacing() == CameraHelper.CameraFacing.CameraFacingFront)
                    	_glesHelper.SetRotationMode(GLESHelper.RotationMode.kRotateRight);
                    else 
                    	_glesHelper.SetRotationMode(GLESHelper.RotationMode.kRotateRightFlipHorizontal);
            		_glesHelper.SetOutputSize(width, height);
            		_glesHelper.SetInputSize(640, 480);
            		_camera.StartPreview(_glesHelper.GetSurfaceTexture(), 640, 480);
                }
            });
		}

		@Override
		public void onDrawFrame(GL10 gl) {
			GLSurfaceViewCus.this.queueEvent(new Runnable(){
            	@Override
                public void run() {
            		_glesHelper.Render();
                }
            });
		}
		
	}
}
