package com.airensoft.jnithreads.app;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends Activity {
    private EditText threadsEdit;
    private EditText iterationsEdit;
    private Button startButton;
    private TextView logView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nativeInit();

        threadsEdit = (EditText)findViewById(R.id.threads_edit);
        iterationsEdit = (EditText)findViewById(R.id.iterations_edit);
        startButton = (Button)findViewById(R.id.start_button);
        logView = (TextView)findViewById(R.id.log_view);

        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int threads = getNumber(threadsEdit, 0);
                int iterations = getNumber(iterationsEdit, 0);

                if(threads > 0 && iterations > 0) {
                    startThreads(threads, iterations);
                }
            }
        });

    }


    @Override
    protected void onDestroy() {
        nativeFree();
        super.onDestroy();
    }

    private void onNativeMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logView.append(message);
                logView.append("\n");
            }
        });
    }

    private static int getNumber(EditText editText, int defaultValue) {
        int value;

        try {
            value = Integer.parseInt(editText.getText().toString());
        } catch (NumberFormatException e) {
            value = defaultValue;
        }

        return value;
    }

    private void startThreads(int threads, int iterations) {
        //javaThreads(threads, iterations);
        posixThreads(threads, iterations);
    }

    private void javaThreads(int threads, final int iterations) {
        for(int i=0; i<threads; i++) {
            final int id = i;

            Thread thread = new Thread() {
                public void run() {
                    nativeWorker(id, iterations);
                }
            };

            thread.start();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private native void nativeInit();
    private native void nativeFree();
    private native void nativeWorker(int id, int iterations);
    private native void posixThreads(int threads, int iterations);

    static {
        System.loadLibrary("JniThreads");
    }
}
