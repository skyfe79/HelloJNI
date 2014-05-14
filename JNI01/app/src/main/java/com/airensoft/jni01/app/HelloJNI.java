package com.airensoft.jni01.app;

/**
 * Created by burt on 2014. 5. 14..
 */
public class HelloJNI {
    static {
        System.loadLibrary("mylib");
    }

    public static native String getHelloString();
}
