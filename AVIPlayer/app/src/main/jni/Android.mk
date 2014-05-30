LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := AVIPlayer
LOCAL_SRC_FILES := common.cpp AVIPlayer.cpp
LOCAL_STATIC_LIBRARIES += avilib_static
LOCAL_LDLIBS += -ljnigraphics

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_LDLIBS += -lGLESv1_CM

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, /Users/burt/AndroidSDK/android-ndk-r9d/sources/transcode-1.1.5/)
$(call import-module, avilib)