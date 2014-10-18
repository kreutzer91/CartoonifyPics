LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include /usr/local/OpenCV-2.4.9-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := native_example
LOCAL_SRC_FILES := jni_part.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)
