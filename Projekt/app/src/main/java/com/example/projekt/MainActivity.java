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
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.api.exception.IncompatibilityException;
import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.IOIOFactory;
import ioio.lib.api.IOIO;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;

public class MainActivity extends AppCompatActivity {
    private static final int DISPLAY_DATA = 1;
    private static int hr = 0;
    public int f_PWM;
    int[] signals;
    IOIO ioio;
    int IOIOstate = 0;
    // this handler will receive a delayed message

    void funConnect(IOIO ioio) throws Exception{

            ioio.waitForConnect();
            IOIOstate = 1;
    }
    void funSetDigital(int pin, boolean state)
    {   try {
        DigitalOutput a;
        a = ioio.openDigitalOutput(pin, true);
        a.write(state);
    }
    catch(Exception e)
    {
        System.out.println("Something wrong");
    }
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        load();
        Ini();

        Button buttonStart = findViewById(R.id.buttonIOIO);
        buttonStart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)  {
                System.out.println("The system have started");
               // Intent i= new Intent(MainActivity.this, OutputActivity.class);
               // startService(i);
                ioio = IOIOFactory.create();
                try{
                    funConnect(ioio);
                }
                catch(Exception e ){
                    System.out.println("Something wrong");
                }
                //zrob tutaj loopa z handlerem 
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
    public native int[] SetSignal();



    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("our-lib");
    }




}
