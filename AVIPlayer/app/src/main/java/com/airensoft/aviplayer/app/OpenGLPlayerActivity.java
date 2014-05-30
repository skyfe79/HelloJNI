package com.airensoft.aviplayer.app;

import android.opengl.GLSurfaceView;
import android.os.Bundle;

import java.util.concurrent.atomic.AtomicBoolean;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by burt on 2014. 5. 29..
 */
public class OpenGLPlayerActivity extends AbstractPlayerActivity {
    private final AtomicBoolean isPlaying = new AtomicBoolean();
    private long instance;
    private GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_gl_player);

        glSurfaceView = (GLSurfaceView)findViewById(R.id.gl_surface_view);
        glSurfaceView.setRenderer(renderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    @Override
    protected void onStart() {
        super.onStart();
        instance = mAVIPlayer.init(avi);
    }

    @Override
    protected void onResume() {
        super.onResume();

        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();

        mAVIPlayer.free(instance);
        instance = 0;
    }

    private final Runnable player = new Runnable() {
        @Override
        public void run() {
            long frameDelay = (long)(1000/mAVIPlayer.getFrameRate(avi));
            while (isPlaying.get()) {
                glSurfaceView.requestRender();
                try {
                    Thread.sleep(frameDelay);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };

    private final GLSurfaceView.Renderer renderer = new GLSurfaceView.Renderer() {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            mAVIPlayer.initSurface(instance, avi);
            isPlaying.set(true);
            new Thread(player).start();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if(!mAVIPlayer.glrender(instance, avi)) {
                isPlaying.set(false);
            }
        }
    };


}
