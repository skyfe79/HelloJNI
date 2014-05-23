package com.airensoft.livecamera.app;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ImageFormat;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.os.Build;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.IOException;
import java.util.List;

/**
 * Created by burt on 2014. 5. 14..
 */
public class CameraView extends SurfaceView implements SurfaceHolder.Callback, Camera.PreviewCallback {
    static {
        System.loadLibrary("livecamera");
    }

    public native void decode(Bitmap pTarget, byte[] pSource);

    private Camera mCamera;
    private byte[] mVideoSource;
    private Bitmap mBackBuffer;
    private Paint mPaint;

    public CameraView(Context context) {
        super(context);

        getHolder().addCallback(this);
        setWillNotDraw(false);
    }

    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        try {
            mCamera = Camera.open(0);
            mCamera.setDisplayOrientation(0);
            mCamera.setPreviewDisplay(null);
            mCamera.setPreviewCallbackWithBuffer(this);
        } catch (IOException e) {
            mCamera.release();
            mCamera = null;
            throw new IllegalStateException();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        mCamera.stopPreview();

        Camera.Size size = findBestResolution(width, height);

        PixelFormat pixelFormat = new PixelFormat();
        PixelFormat.getPixelFormatInfo(mCamera.getParameters().getPreviewFormat(), pixelFormat);

        int sourceSize = size.width * size.height * pixelFormat.bitsPerPixel / 8;
        mVideoSource = new byte[sourceSize];
        mBackBuffer = Bitmap.createBitmap(size.width, size.height, Bitmap.Config.ARGB_8888);

        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(size.width, size.height);
        parameters.setPreviewFormat(ImageFormat.NV21);
        mCamera.setParameters(parameters);

        mCamera.addCallbackBuffer(mVideoSource);
        mCamera.startPreview();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if(mCamera != null) {
            mCamera.stopPreview();
            mCamera.release();

            mCamera = null;
            mVideoSource = null;
            mBackBuffer = null;
        }
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        decode(mBackBuffer, data);
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if(mCamera != null) {
            canvas.drawBitmap(mBackBuffer, 0, 0, mPaint);
            mCamera.addCallbackBuffer(mVideoSource);
        }
    }

    private Camera.Size findBestResolution(int pWidth, int pHeight) {
        List<Camera.Size> lSizes = mCamera.getParameters()
                .getSupportedPreviewSizes();
        // Finds the biggest resolution which fits the screen.
        // Else, returns the first resolution found.
        Camera.Size lSelectedSize = mCamera.new Size(0, 0);
        for (Camera.Size lSize : lSizes) {
            if ((lSize.width <= pWidth)
                    && (lSize.height <= pHeight)
                    && (lSize.width >= lSelectedSize.width)
                    && (lSize.height >= lSelectedSize.height)) {
                lSelectedSize = lSize;
            }
        }
        // Previous code assume that there is a preview size smaller
        // than screen size. If not, hopefully the Android API
        // guarantees that at least one preview size is available.
        if ((lSelectedSize.width == 0)
                || (lSelectedSize.height == 0)) {
            lSelectedSize = lSizes.get(0);
        }
        return lSelectedSize;
    }
}
