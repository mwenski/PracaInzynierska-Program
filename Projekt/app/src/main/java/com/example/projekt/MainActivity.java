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

   public class MainActivity extends IOIOService {

       @Nullable
       @Override
       public IBinder onBind(Intent intent) {
           return null;
       }

       //  @Override
      //  protected void onCreate(Bundle savedInstanceState) {
      //      super.onCreate(savedInstanceState);
            //  setContentView(R.layout.activity_main);
      //  }
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
        class Looper extends BaseIOIOLooper {


            private DigitalOutput a;
            private PwmOutput b;
            private AnalogInput c;
            private DigitalInput d;
            private PulseInput pulse;
            private float freqHz;
            public int f_PWM = 25000;

            //Ta funkcja działa jak setup w Arduino
            @Override
            protected void setup() throws ConnectionLostException {
               // showVersions(ioio_, "Połączono z IOIO!");
                try {
                    /**Inicjalizacja działań płytki tutaj*/
                    Ini();
                    a = ioio_.openDigitalOutput(0,true);
                    b = ioio_.openPwmOutput(new DigitalOutput.Spec(14, DigitalOutput.Spec.Mode.OPEN_DRAIN), f_PWM);
                    pulse = ioio_.openPulseInput(11, PulseInput.PulseMode.FREQ);
                    for(int p=0; p<3; p++) {
                        a.write(false);
                        Thread.sleep(300);
                        a.write(true);
                        Thread.sleep(300);
                    }
                    freqHz = 0;
                    System.out.println(Con(1, 2));
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    ioio_.disconnect();
                }
            }

            //Ta funkcja działa jak loop w Arduino
            @Override
            public void loop() throws ConnectionLostException {
                try {
                    /**Sterowanie w pętli tutaj*/
                        a.write(false);
                        b.setDutyCycle(Con(f_PWM, freqHz));
                        b.setDutyCycle(0);
                       // Thread.sleep(5000);
                        float pulseSeconds = pulse.getDuration();
                        float freqHz = pulse.getFrequency();
                        Log.d("TACHOMETER READING", "Last impulse duration [s]: " + pulseSeconds + "; Frequency [Hz]: " + freqHz);
                       // Thread.sleep((1/f_PWM)*1000);
                        Thread.sleep(100);

                } catch (InterruptedException e) {
                    ioio_.disconnect();
                }
            }

            //funkcja wywoływana, gdy połączenie zostanie przerwane
            @Override
            public void disconnected() {
                System.out.println("The system have started");
            }

            //funkcja wywoływana, gdy wersja oprogramowania jest nieprawidłowa
            @Override
            public void incompatible() {
            }
        }

        @Override
        protected IOIOLooper createIOIOLooper() {
            return new Looper();
        }

        /*
        //Dolny pasek
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

*/

    }


