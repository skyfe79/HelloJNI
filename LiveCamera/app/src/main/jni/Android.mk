LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := livecamera
LOCAL_SRC_FILES := livecamera.c
LOCAL_LDLIBS    += -ljnigraphics

include $(BUILD_SHARED_LIBRARY)