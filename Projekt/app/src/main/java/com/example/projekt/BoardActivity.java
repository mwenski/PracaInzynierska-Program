package com.example.projekt;

import android.content.Context;
import android.os.Bundle;
import android.widget.Toast;

import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.IOIO;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;


public class BoardActivity extends IOIOActivity{

 /*   @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
 */

    class Looper extends BaseIOIOLooper {

        private DigitalOutput a;
        private PwmOutput b;
        private AnalogInput c;
        private DigitalInput d;


        //Ta funkcja działa jak setup w Arduino
        @Override
        protected void setup() throws ConnectionLostException {
            showVersions(ioio_, "Połączono z IOIO!");
            try{
                /**Inicjalizacja działań płytki tutaj*/







                Thread.sleep(100);
            }catch(InterruptedException e){
                ioio_.disconnect();
            }
        }

        //Ta funkcja działa jak loop w Arduino
        @Override
        public void loop() throws ConnectionLostException {
            try{
         /**Sterowanie w pętli tutaj*/







                Thread.sleep(100);
            }catch(InterruptedException e){
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
            showVersions( ioio_, "Nieprawidłowa wersja oprogramowania!");
        }
    }

    @Override
    protected IOIOLooper createIOIOLooper() {
        return new Looper();
    }

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



}
