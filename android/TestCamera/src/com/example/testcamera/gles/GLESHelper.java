package com.example.testcamera.gles;

import javax.microedition.khronos.opengles.GL10;

import com.example.testcamera.jni.TestCameraLib;

import android.annotation.SuppressLint;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;

@SuppressLint("InlinedApi") 
public class GLESHelper {
    private SurfaceTexture _surfaceTexture = null;
    private int[] _textureID = new int[1];

    public GLESHelper() {
    	this(null);
    }
    public GLESHelper(OnFrameAvailableListener listener) {
    	super();
    	TestCameraLib.Init();
    	SetOnFrameAvailableListener(listener);
    }
    public SurfaceTexture GetSurfaceTexture() {
    	this.initTexture();
    	return _surfaceTexture;
    }
    public void SetOnFrameAvailableListener(OnFrameAvailableListener listener) {
    	this.initTexture();
 		_surfaceTexture.setOnFrameAvailableListener(listener); 
    }
    public void SetOutputSize(int w, int h) {
    	TestCameraLib.SetOutputSize(w, h);
    }
    public void SetInputSize(int w, int h) {
    	TestCameraLib.SetInputSize(w, h);
    }
    public void SetRotationMode(RotationMode mode) {
    	TestCameraLib.SetRotationMode(mode.value);
    }
    public void Render() {
    	TestCameraLib.Draw();
    	_surfaceTexture.updateTexImage();
    }
    
    
    protected void finalize() {
    	releaseTexture();
    }
    
	private void initTexture() {
     	if(_surfaceTexture == null) {
     		createTexture();
     		_surfaceTexture = new SurfaceTexture(_textureID[0]);
     	} 
	}
	
	private void createTexture()  
    {  
		if(_textureID[0] <= 0) {
	        GLES20.glGenTextures(1, _textureID, 0);  
	        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, _textureID[0]);  
	        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,  
	                GL10.GL_TEXTURE_MIN_FILTER,GL10.GL_LINEAR);          
	        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,  
	                GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);  
	        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,  
	                GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);  
	        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,  
	                GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE); 
	        
	        TestCameraLib.SetTexture(_textureID[0]);
		} 
    }  
	private void releaseTexture() {
		if(_textureID[0] > 0) {
			GLES20.glDeleteTextures(1, _textureID, 0);
			_textureID[0] = 0;
			
	        TestCameraLib.SetTexture(_textureID[0]);
		}
	}

    public enum RotationMode{
    	kNoRotation					(0),
    	kRotateLeft					(1),
    	kRotateRight   				(2),
    	kFlipVertical				(3),
    	kFlipHorizonal 				(4),
    	kRotateRightFlipVertical	(5),
    	kRotateRightFlipHorizontal	(6),
    	kRotate180					(7);
    	private int value = 0;
        private RotationMode(int value) 
        {
            this.value = value;
        }
        public int value() 
        {
            return this.value;
        }
    }
}
