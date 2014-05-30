package com.airensoft.aviplayer.app;

import android.app.Activity;
import android.app.AlertDialog;

import java.io.IOException;

/**
 * Created by burt on 2014. 5. 29..
 */
public class AbstractPlayerActivity extends Activity {
    public static final String EXTRA_FILE_NAME = "com.airensoft.aviplayer.app.EXTRA_FILE_NAME";
    protected long avi = 0;
    AVIPlayer mAVIPlayer;

    @Override
    protected void onStart() {
        super.onStart();

        mAVIPlayer = new AVIPlayer();

        try {
            avi = mAVIPlayer.open(getFileName());
        } catch (IOException e) {
            new AlertDialog.Builder(this).setTitle(R.string.error_alert_title).setMessage(e.getMessage()).show();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        if(0 != avi) {
            mAVIPlayer.close(avi);
            avi = 0;
        }
    }

    protected String getFileName() {
        return getIntent().getExtras().getString(EXTRA_FILE_NAME);
    }
}
