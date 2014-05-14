package com.airensoft.store.app;

import com.airensoft.store.app.exception.InvalidTypeException;
import com.airensoft.store.app.exception.NotExistingKeyException;

/**
 * Created by burt on 2014. 5. 14..
 */
public class Store {
    static {
        System.loadLibrary("store");
    }

    public native int getInteger(String key)
            throws NotExistingKeyException, InvalidTypeException;
    public native void setInteger(String key, int value);

    public native String getString(String key)
            throws NotExistingKeyException, InvalidTypeException;
    public native void setString(String key, String value);

    public native Color getColor(String key)
            throws NotExistingKeyException, InvalidTypeException;
    public native void setColor(String key, Color value);

    public native int[] getIntegerArray(String pKey)
        throws NotExistingKeyException;
    public native void setIntegerArray(String pKey, int[] pIntArray);

    public native Color[] getColorArray(String pKey)
        throws NotExistingKeyException;
    public native void setColorArray(String pKey, Color[] pColorArray);
}
