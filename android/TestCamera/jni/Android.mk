LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := TestCamera
LOCAL_SRC_FILES := TestCamera.c \
				TestRenderer.c \
				glesHelper.c \
				
				
LOCAL_LDLIBS     := -L$(SYSROOT)/usr/lib -llog -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
