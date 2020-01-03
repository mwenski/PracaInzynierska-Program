package ioio.examples.hello;

import ioio.lib.api.AnalogInput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.DigitalOutput;
import ioio.lib.api.IOIO;
import ioio.lib.api.PwmOutput;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOActivity;


import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.widget.TextView;



public class MainActivity extends IOIOActivity implements SensorEventListener{

	private ToggleButton toggleButton1;
	private Button button1;
	private SeekBar seekBar1;
	private int seek_bar_value_1;

	private final int TILT_PIN = 3;
	private final int PAN_PIN = 6;
	private final int SEEK_PIN = 7;

	private final int PWM_FREQ = 100;

	private int mXValue = 500;
	private int mYValue = 500;
	/* sensor data */
	SensorManager m_sensorManager;
	float[] m_lastMagFields;
	float[] m_lastAccels;
	private float[] m_rotationMatrix = new float[16];
	//private float[] m_remappedR = new float[16];
	private float[] m_orientation = new float[4];

	/* fix random noise by averaging tilt values */
	final static int AVERAGE_BUFFER = 30;
	float[] m_prevPitch = new float[AVERAGE_BUFFER];
	float m_lastPitch = 0.f;
	float m_lastYaw = 0.f;
	/* current index int m_prevEasts */
	int m_pitchIndex = 0;

	float[] m_prevRoll = new float[AVERAGE_BUFFER];
	float m_lastRoll = 0.f;
	/* current index into m_prevTilts */
	int m_rollIndex = 0;

	/* center of the rotation */
//	private float m_tiltCentreX = 0.f;
//	private float m_tiltCentreY = 0.f;
//	private float m_tiltCentreZ = 0.f;


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		toggleButton1 = (ToggleButton) findViewById(R.id.toggleButton1);
		button1 = (Button) findViewById(R.id.button1);

		seekBar1 = (SeekBar) findViewById(R.id.seekBar1);
/////////////////////////////////////////////////////////WYWOŁYWANIE SENSORÓW (TEST)
		//Funkcje sensorów w Javie (Jak działają, to później usunę)
		m_sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		registerListeners();
	}

	private void registerListeners() {
		m_sensorManager.registerListener(this,
				m_sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD),
				SensorManager.SENSOR_DELAY_GAME);
		m_sensorManager.registerListener(this,
				m_sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
				SensorManager.SENSOR_DELAY_GAME);
	}

	private void unregisterListeners() {
		m_sensorManager.unregisterListener(this);
	}

	@Override
	public void onDestroy() {
		unregisterListeners();
		super.onDestroy();
	}

	@Override
	public void onPause() {
		unregisterListeners();
		super.onPause();
	}

	@Override
	public void onResume() {
		registerListeners();
		super.onResume();
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}
//Przykładowe wywoływanie obliczania sygnału dla silinika/serwo przy zmianie danych z sensorów (ODTĄD WAŻNE)
	@Override
	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
			accel(event);
		}
		if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
			mag(event);
		}
	}

	private void accel(SensorEvent event) {
		if (m_lastAccels == null) {
			m_lastAccels = new float[3];
		}

		System.arraycopy(event.values, 0, m_lastAccels, 0, 3);

		/*
		 * if (m_lastMagFields != null) { computeOrientation(); }
		 */
	}

	private void mag(SensorEvent event) {
		if (m_lastMagFields == null) {
			m_lastMagFields = new float[3];
		}

		System.arraycopy(event.values, 0, m_lastMagFields, 0, 3);

		if (m_lastAccels != null) {
			//O TUTAJ
			computeOrientation();
		}
	}

	////////////////////////////////////////////////////////KONIEC Z SENSORAMI

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		switch (item.getItemId())
		{
			case R.id.action_about:
				//Toast.makeText(getApplicationContext(), "Show About", Toast.LENGTH_SHORT).show();

				Intent about = new Intent(this, AboutActivity.class);
				startActivity(about);

				return true;
			default:
				return super.onOptionsItemSelected(item);
		}
	}

	//WYZNACZANIE SYGNAŁU WEJŚCIA (funkcje)
	Filter[] m_filters = { new Filter(), new Filter(), new Filter() };

	private class Filter {
		static final int AVERAGE_BUFFER = 10;
		float[] m_arr = new float[AVERAGE_BUFFER];
		int m_idx = 0;

		public float append(float val) {
			m_arr[m_idx] = val;
			m_idx++;
			if (m_idx == AVERAGE_BUFFER)
				m_idx = 0;
			return avg();
		}

		public float avg() {
			float sum = 0;
			for (float x : m_arr)
				sum += x;
			return sum / AVERAGE_BUFFER;
		}

	}

	private void computeOrientation() {
		if (SensorManager.getRotationMatrix(m_rotationMatrix, null, m_lastMagFields, m_lastAccels)) { //Macierz rotacji (Do zmiany)
			SensorManager.getOrientation(m_rotationMatrix, m_orientation); //Wyznaczanie orientacji z macierzy (Do zmiany)

			/* 1 radian = 57.2957795 stopnia */
			/*
			 * [0] : yaw, Rotacja wokół osi z
			 * [1] : pitch, Rotacja wokół osi x
			 * [2] : roll, Rotacja wokół osi y
			 */
			float yaw = m_orientation[0] * 57.2957795f;
			float pitch = m_orientation[1] * 57.2957795f;
			float roll = m_orientation[2] * 57.2957795f;

			m_lastYaw = m_filters[0].append(yaw);
			m_lastPitch = m_filters[1].append(pitch);
			m_lastRoll = m_filters[2].append(roll);
			TextView rt = (TextView) findViewById(R.id.roll);
			TextView pt = (TextView) findViewById(R.id.pitch);
			TextView yt = (TextView) findViewById(R.id.yaw);

			int servo = 50;
			if (m_lastRoll >= 0) {
				servo = (int) (50 + (180 - m_lastRoll));
			} else {
				// servo1 = (int) (50 - (180 + m_lastRoll));
				servo = 0;
			}

			int motor = 50;
			if (m_lastYaw > 100) {
				motor = 100;
			} else if (m_lastYaw >= 0) {
				motor = (int) m_lastYaw;
			} else {
				motor = 0;
			}

			rt.setText("Roll y (Pin 6): " + m_lastRoll + " Servo1: " + servo);
			pt.setText("Pitch x: " + m_lastPitch);
			yt.setText("Azimuth z(Pin 3): " + m_lastYaw + " Servo2: " + motor);

			mXValue = servo * 10;
			mYValue = motor * 10;
		}
	}

//KLASA OBSŁUGUJĄCA IOIO
	class Looper extends BaseIOIOLooper {

		private DigitalOutput digital_led1;
		private PwmOutput servoPwmOutput;
		private PwmOutput panPwmOutput;
		private PwmOutput tiltPwmOutput;
		private AnalogInput czujnikA;
		private DigitalInput czujnikD;


		//Ta funkcja działa jak setup w Arduino
		@Override
		protected void setup() throws ConnectionLostException {
			showVersions(ioio_, "Połączono z IOIO!");

			digital_led1 = ioio_.openDigitalOutput(0,true); //Otwieramy Pin 0 Cyfrowy (Dioda na IOIO)
			servoPwmOutput = ioio_.openPwmOutput(SEEK_PIN, PWM_FREQ); //Otwieramy Pin 7 PWM, częstotliwość 100Hz
			panPwmOutput = ioio_.openPwmOutput(PAN_PIN, PWM_FREQ); //Pin 6
			tiltPwmOutput = ioio_.openPwmOutput(TILT_PIN, PWM_FREQ); //Pin 3

			//Czujnik analogowy otwarcie
			czujnikA = ioio_.openAnalogInput(40);

			//Czujnik cyfrowy otwarcie
			czujnikD = ioio_.openDigitalInput(37,DigitalInput.Spec.Mode.PULL_UP);

			// Ustawia wartość na środek paska w apce
			button1.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					seekBar1.setProgress(50);
					seek_bar_value_1 = 50;
				}
			});

			// SeekBar1
			seekBar1.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {
					// TODO Auto-generated method stub

				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {
					// TODO Auto-generated method stub

				}

				@Override
				public void onProgressChanged(SeekBar seekBar, int progress,
											  boolean fromUser) {
					// TODO Auto-generated method stub

					seek_bar_value_1 = progress;
					Log.d("Servo ", " = "+ seek_bar_value_1);
				}
			});


		}

	//Ta funkcja działa jak loop w Arduino
		@Override
		public void loop() throws ConnectionLostException {

			digital_led1.write(!toggleButton1.isChecked()); //Włącza diodę na IOIO (Pin 0)

			servoPwmOutput.setPulseWidth((int)((seek_bar_value_1 * 10)+1000)); //Sterowanie szerokością impulsu (prędkością)
			panPwmOutput.setPulseWidth(500 + mXValue * 2);
			tiltPwmOutput.setPulseWidth(500 + mYValue * 2);

			try{
				//Odczyt z czujnika analogowego
				final float dane_analogowe = czujnikA.read(); //odczyt między 0 a 1
				final float napiecie_analogowe = czujnikA.getVoltage(); //odczyt napięcia

				//Odczyt z czujnika cyfrowego
				boolean value = czujnikD.read();

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

	//Tu powstaje nowy wątek
	@Override
	protected IOIOLooper createIOIOLooper() {
		return new Looper();
	}

	//Tu jest powstaje widget z komunikatem
	private void toast(final String message) {
		final Context context = this;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(context, message, Toast.LENGTH_LONG).show();
			}
		});
	}

	//Tu powstaje widget z danymi o IOIO
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
