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
import ioio.lib.api.IOIOConnection;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;
import ioio.lib.util.IOIOConnectionRegistry;
import ioio.lib.util.IOIOConnectionManager;

public class UIActivity extends AppCompatActivity {
    private static final int DISPLAY_DATA = 1;
    boolean s = false;
    public static TextView out;
    public static UIActivity instanceUI;
    // this handler will receive a delayed message
    public static void upDate(String a)
    {
        final String b = a;
        instanceUI.runOnUiThread(new Runnable() {
            public void run() {
        out.setText(b);
            }
    });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        instanceUI = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        out = findViewById(R.id.tach_text);
        Button buttonIn = findViewById(R.id.DECREASE);
        Button buttonDec = findViewById(R.id.INCREASE);
        Button buttonStart = findViewById(R.id.buttonIOIO);
        config.load(this); //wczytaj
        buttonIn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)  {
                Infun();
            }
        });
        buttonDec.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v)  {
                    Decfun();
                }
            });
        buttonStart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v)  {
                Intent i= new Intent(UIActivity.this, MainActivity.class);
                if(s==false){
                    System.out.println("The system have started");
                    // Intent i= new Intent(UIActivity.this, MainActivity.class);
                    startService(i);
                    s=true;
                }else{
                    stopService(i);
                    s=false;
                }
            }
        });
    }
  
    public void Click(View view) {
        switch(view.getId()){
            case R.id.button: {
                MainActivity.Calibration();
                break;
            }
        }
    }
    public void Decfun() {
        MainActivity.instance. dec();
    }
        public void Infun() {
                MainActivity.instance.inc();
                }




    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */





}
