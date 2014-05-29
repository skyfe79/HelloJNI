LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := JniThreads
LOCAL_SRC_FILES := JniThreads.cpp

include $(BUILD_SHARED_LIBRARY)