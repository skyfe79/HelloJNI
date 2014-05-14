package com.airensoft.store.app;

/**
 * Created by burt on 2014. 5. 14..
 */
public class Color {
    private int mColor;

    public Color(String colorString) {
        super();
        mColor = android.graphics.Color.parseColor(colorString);
    }

    @Override
    public String toString() {
        return String.format("#%06X", mColor);
    }
}
