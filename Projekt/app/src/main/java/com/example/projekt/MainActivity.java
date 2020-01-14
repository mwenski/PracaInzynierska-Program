package com.example.projekt;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.IOException;
import java.io.InputStream;

import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.IOIO;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;

public class MainActivity extends AppCompatActivity {
    private static final int DISPLAY_DATA = 1;
    private static TextView tv[]=new TextView[4];
    private static int hr = 0;
    public int f_PWM;
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

        Button buttonStart = findViewById(R.id.buttonIOIO);
        buttonStart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                System.out.println("The system have started");
                Intent i= new Intent(MainActivity.this, OutputActivity.class);
                startService(i);
            }
        });
    }
  
    public void Click(View view) {
        switch(view.getId()){
            case R.id.button: {
                Calibration();
                break;
            }
        }


    }
    public String readJSON() {//parsuje
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
        int gravitation;

        //String name;
        try {
            JSONArray jArray = new JSONArray(readJSON());
            gravitation=jArray.getJSONObject(0).getInt("number");
            f_PWM = jArray.getJSONObject(1).getInt("number");
            //name=jArray.getJSONObject(1).getString("name2");

        } catch (JSONException e) {
            gravitation=0;
            f_PWM = 0;
        }
        load(gravitation);
        // load(f_PWM);
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
    public native void Reading(int i);
    public native int SetSignal();


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("our-lib");
    }
/*
    public class CommunicateWithIOIO extends IOIOActivity {

        class Looper extends BaseIOIOLooper {

            private DigitalOutput a;
            private PwmOutput b;
            private AnalogInput c;
            private DigitalInput d;


            @Override
            protected void setup() throws ConnectionLostException {
                showVersions(ioio_, "Połączono z IOIO!");
                try {
                    //Inicjalizacja działań płytki tutaj

                    b = ioio_.openPwmOutput(3, f_PWM);
                    a = ioio_.openDigitalOutput(0,true);






                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    ioio_.disconnect();
                }
            }

            @Override
            public void loop() throws ConnectionLostException {
                try {
                    //Sterowanie w pętli tutaj

                    b.setPulseWidth(SetSignal());
                    a.write(false);





                    Reading(0);

                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    ioio_.disconnect();
                }
            }

            //funkcja wywoływana, gdy połączenie zostanie przerwane
            @Override
            public void disconnected() {
                toast("IOIO zostało odłączone");
            }

            //funkcja wywoływana, gdy wersja oprogramowania jest nieprawidłowa
            @Override
            public void incompatible() {
                showVersions(ioio_, "Nieprawidłowa wersja oprogramowania!");
            }
        }

        @Override
        protected IOIOLooper createIOIOLooper() {
            return new Looper();
        }

        //Widget z komunikatami
        private void toast(final String message) {
            final Context context = this;
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(context, message, Toast.LENGTH_LONG).show();
                }
            });
        }

        //Dane o IOIO
        private void showVersions(IOIO ioio, String title) {
            toast(String.format("%s\n" +
                            "IOIOLib: %s\n" +
                            "Application firmware: %s\n" +
                            "Bootloader firmware: %s\n" +
                            "Hardware: %s",
                    title,
                    ioio.getImplVersion(IOIO.VersionType.IOIOLIB_VER),
                    ioio.getImplVersion(IOIO.VersionType.APP_FIRMWARE_VER),
                    ioio.getImplVersion(IOIO.VersionType.BOOTLOADER_VER),
                    ioio.getImplVersion(IOIO.VersionType.HARDWARE_VER)));
        }

    }



*/
}
