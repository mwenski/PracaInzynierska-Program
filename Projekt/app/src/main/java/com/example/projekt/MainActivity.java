package com.example.projekt;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;
import android.os.Handler;
import android.os.Message;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {
    private static final int DISPLAY_DATA = 1;
    private static TextView tv[]=new TextView[4];
    private static int hr = 0;
    // this handler will receive a delayed message
    private Handler mHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            load();
            if (msg.what == DISPLAY_DATA){
                tv[0] = findViewById(R.id.sample_text1);
                tv[1]= findViewById(R.id.sample_text2);
                tv[2] = findViewById(R.id.sample_text3);
                tv[3] = findViewById(R.id.sample_text4);
            }
            tv[0].setText(Update(1));
            tv[1].setText(Update(2));
            tv[2].setText(Update(3));
            tv[3].setText(Update(4));
            mHandler.sendEmptyMessageDelayed(DISPLAY_DATA, 50);
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        load();
        //Trajectory(Environment.getExternalStorageDirectory().getPath());
        Ini();
        mHandler.sendEmptyMessageDelayed(DISPLAY_DATA, 100);
    }
    public void Click(View view) {
        switch(view.getId()){
            case R.id.button: {
                Calibration();
                break;
            }
        }

    }
    public String readJSONFromAsset() {//parsuje
        String json = null;
        try {
            InputStream in = getAssets().open("plik.json");
            int size = in.available();
            byte[] bufor = new byte[size];
            in.read(bufor);
            in.close();
            json = new String(bufor, "UTF-8");
        } catch (IOException ex) {
            ex.printStackTrace();
            return null;
        }
        return json;
    }
    public void load() {
        int num;
        //String name;
        try {
            JSONArray jArray = new JSONArray(readJSONFromAsset());
           num=jArray.getJSONObject(0).getInt("liczba");
            //name=jArray.getJSONObject(1).getString("name2");

        } catch (JSONException e) {
         num=0;
        }
        load(num);
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String Update(int i);
    public native void Trajectory(String fp);
    public native void Ini();
    public native void Calibration();
    public native void load(int a);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("our-lib");
    }


}
