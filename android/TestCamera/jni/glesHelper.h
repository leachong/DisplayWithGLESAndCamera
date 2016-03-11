

#ifndef GLESHELPER_H_
#define GLESHELPER_H_

#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdint.h>

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

typedef enum {
	kGPUImageNoRotation 				= 0,
	kGPUImageRotateLeft 				= 1,
	kGPUImageRotateRight 				= 2,
	kGPUImageFlipVertical 				= 3,
	kGPUImageFlipHorizonal 				= 4,
	kGPUImageRotateRightFlipVertical 	= 5,
	kGPUImageRotateRightFlipHorizontal 	= 6,
	kGPUImageRotate180 					= 7,
} GPUImageRotationMode;

#define DEBUG 1

#define LOG_TAG "TestCameraLib"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef DEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define LOGD(...)
#endif

#define GPUImageRotationSwapsWidthAndHeight(rotation) ((rotation) == kGPUImageRotateLeft || (rotation) == kGPUImageRotateRight || (rotation) == kGPUImageRotateRightFlipVertical || (rotation) == kGPUImageRotateRightFlipHorizontal)

BOOL checkGlError(const char* funcName);

GLuint createProgram(const char* vtxSrc, const char* fragSrc);

const float* getRotation(GPUImageRotationMode rotationMode);

GLuint createTexture(uint8_t* data, int width, int height);

#endif /* GLRENDER_H_ */
