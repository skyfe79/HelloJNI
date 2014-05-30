package com.airensoft.aviplayer.app;

import android.graphics.Bitmap;

import java.io.IOException;

/**
 * Created by burt on 2014. 5. 29..
 */
public class AVIPlayer {
    static {
        System.loadLibrary("AVIPlayer");
    }

    public native static long open(String fileName) throws IOException;
    public native static int getWidth(long avi);
    public native static int getHeight(long avi);
    public native static double getFrameRate(long avi);
    public native static void close(long avi);
    public native static boolean render(long avi, Bitmap bitmap);
    public native static boolean glrender(long instance, long avi);
    public native static long init(long avi);
    public native static void initSurface(long instance, long avi);
    public native static void free(long instance);
}
