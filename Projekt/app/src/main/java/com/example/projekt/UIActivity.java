package com.example.projekt;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

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
        Button buttonDec = findViewById(R.id.decreaser);
        Button buttonIn = findViewById(R.id.increaser);
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
        MainActivity.instance.dec();
        System.out.println("FREQ IS:" + MainActivity.instance.targetFreqHz);
    }
    public void Infun() {
        MainActivity.instance.inc();
        System.out.println("FREQ IS:" + MainActivity.instance.targetFreqHz);
    }




    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */





}
