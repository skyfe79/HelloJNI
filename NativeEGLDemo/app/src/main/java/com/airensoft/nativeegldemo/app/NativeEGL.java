package com.airensoft.nativeegldemo.app;

import android.view.Surface;

/**
 * Created by burt on 2014. 5. 23..
 */
public class NativeEGL {
    public static native void nativeOnStart();
    public static native void nativeOnResume();
    public static native void nativeOnPause();
    public static native void nativeOnStop();
    public static native void nativeSetSurface(Surface surface);

    static {
        System.loadLibrary("nativeegl");
    }
}
