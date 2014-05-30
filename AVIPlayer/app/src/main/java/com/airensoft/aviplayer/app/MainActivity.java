package com.airensoft.aviplayer.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;

import java.io.File;


public class MainActivity extends Activity implements View.OnClickListener {

    private EditText fileNameEdit;
    private RadioGroup playerRadioGroup;
    private Button playButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        fileNameEdit = (EditText)findViewById(R.id.file_name_edit);
        playerRadioGroup = (RadioGroup)findViewById(R.id.player_radio_group);
        playButton = (Button)findViewById(R.id.play_button);
        playButton.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.play_button:
                onPlayButtonClick();
                break;
        }
    }


    private void onPlayButtonClick() {
        Intent intent;
        int radioId = playerRadioGroup.getCheckedRadioButtonId();

        switch (radioId) {
            case R.id.bitmap_player_radio:
                intent = new Intent(this, BitmapPlayerActivity.class);
                break;
            case R.id.open_gl_player_radio:
                intent = new Intent(this, OpenGLPlayerActivity.class);
                break;
            default:
                throw new UnsupportedOperationException("radioId=" + radioId);
        }

        File file = new File(Environment.getExternalStorageDirectory(), fileNameEdit.getText().toString());
        intent.putExtra(AbstractPlayerActivity.EXTRA_FILE_NAME, file.getAbsolutePath());
        startActivity(intent);
    }
}
