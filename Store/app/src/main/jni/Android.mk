LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DHAVE_INTTYPES_H
LOCAL_MODULE := store
LOCAL_SRC_FILES := mystorelib.c Store.c

include $(BUILD_SHARED_LIBRARY)