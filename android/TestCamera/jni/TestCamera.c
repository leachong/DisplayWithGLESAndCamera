#include <jni.h>
#include "TestRenderer.h"

JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_Init(JNIEnv * env, jobject obj)
{
	Renderer_Init();
}

JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_SetRotationMode(JNIEnv * env, jobject obj, jint mode)
{
	Renderer_SetRotationMode(mode);
}
JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_SetOutputSize(JNIEnv * env, jobject obj, jint width, jint height)
{
	Renderer_SetOutputSize(width,height);
}
JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_SetTexture(JNIEnv * env, jobject obj, jint txtid)
{
	Renderer_SetTexture(txtid);
}
JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_SetInputSize(JNIEnv * env, jobject obj,  jint width, jint height)
{
	Renderer_SetInputSize(width,height);
}

JNIEXPORT void JNICALL Java_com_example_testcamera_jni_TestCameraLib_Draw(JNIEnv * env, jobject obj)
{
	Renderer_Draw();
}
