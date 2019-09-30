package com.example.projekt;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.os.Handler;
import android.os.Message;
public class MainActivity extends AppCompatActivity {
    private static final int DISPLAY_DATA = 1;
    private static TextView tv;
    private static int hr = 0;
    // this handler will receive a delayed message
    private Handler mHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            if (msg.what == DISPLAY_DATA)  tv = findViewById(R.id.sample_text);
            tv.setText(Update());
            mHandler.sendEmptyMessageDelayed(DISPLAY_DATA, 100);
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Ini();
        mHandler.sendEmptyMessageDelayed(DISPLAY_DATA, 100);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String Update();
    public native void Ini();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
