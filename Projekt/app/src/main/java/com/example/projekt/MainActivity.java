package com.example.projekt;
//import android.app.Service;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.widget.Toast;
//import android.app.Notification;
//import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.util.Log;
import androidx.annotation.Nullable;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import org.json.JSONArray;
import org.json.JSONException;

import java.io.Console;
import java.io.IOException;
import java.io.InputStream;

import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.IOIO;
import ioio.lib.api.PulseInput;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOService;
import java.util.Timer;
import java.util.TimerTask;
public class MainActivity extends IOIOService {

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    static {
        System.loadLibrary("our-lib");
    }

    public static native float Con(float in, float read);
    public static native String Update(int i);
    public static native void Trajectory(String fp);
    public static native void Ini();
    public static native void Reading(int i);
    public static native int[] SetSignal();
    public static native void load(int a);
    public static native void Calibration();
    public static float tacho(){
        return instance.freqHz;
    }
    public static Looper instance;
    class Looper extends BaseIOIOLooper {


        private DigitalOutput a;
        private PwmOutput b;
        private AnalogInput c;
        private DigitalInput d;
        private PulseInput pulse;
        public float freqHz;
        public float targetFreqHz;
        public boolean synct = true;
        private double Tp = 100; //w milisekundach
        private Timer tim;
        public int f_PWM = 25000;
        public void dec(){
            targetFreqHz = targetFreqHz - (float)1;
            if (targetFreqHz < 0)
                targetFreqHz = 0;
        }
        public void inc(){
            targetFreqHz = targetFreqHz + (float)1;
            if (targetFreqHz > 150)
                targetFreqHz = 150;
        }

        class Helper extends TimerTask
        {
            public void run()
            {
                synct = true;
            }
        }
        //Funkcja inicjalizująca
        @Override
        protected void setup() throws ConnectionLostException {
            try {
                instance=this;
                Ini();
                a = ioio_.openDigitalOutput(0,true);
                b = ioio_.openPwmOutput(new DigitalOutput.Spec(14, DigitalOutput.Spec.Mode.OPEN_DRAIN), f_PWM);
                DigitalInput.Spec Spek = new DigitalInput.Spec(11,DigitalInput.Spec.Mode.FLOATING);
                pulse = ioio_.openPulseInput(Spek , PulseInput.ClockRate.RATE_250KHz,PulseInput.PulseMode.FREQ,true);
                for(int p=0; p<3; p++) {
                    a.write(false);
                    Thread.sleep(300);
                    a.write(true);
                    Thread.sleep(300);
                }
                freqHz = 0;
                targetFreqHz = 50;
                Thread.sleep(100);
                tim = new Timer(true);
                TimerTask task = new Helper();
                tim.schedule(task,100, (long) Tp);
            } catch (InterruptedException e) {
                ioio_.disconnect();
            }
        }

        //Funkcja działająca w pętli
        @Override
        public void loop() throws ConnectionLostException {
            try {
                if(synct == true) {
                    synct = false;
                    a.write(false);
                    float pulseSeconds = pulse.getDuration();
                    freqHz = pulse.getFrequency();
                    b.setDutyCycle(Con(targetFreqHz, freqHz));
                   // b.setDutyCycle(1);
                    UIActivity.upDate("RPM is: " + freqHz * 60 / 2);
                    Log.d("TACHOMETER READING", "Last impulse duration [s]: " + pulseSeconds + "; Frequency [Hz]: " + freqHz +" TargetFrequency [Hz]"+targetFreqHz);
                }

            } catch (InterruptedException e) {
                ioio_.disconnect();
            }
        }


        @Override
        public void disconnected() {
            System.out.println("IOIO has been disconnected");
        }


        @Override
        public void incompatible() {
            System.out.println("Incompatible version of software");
        }
    }

    @Override
    protected IOIOLooper createIOIOLooper() {
        instance = new Looper();
        return instance;
        //return new Looper();
    }



}

