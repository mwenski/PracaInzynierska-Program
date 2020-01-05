#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include"config.h"
#include"sensor.h"
#include"data.h"
#include"trajectory.h"
#include "control.h"
#include <pthread.h>
#include <time.h>
//czas w ms
ASensorEventQueue* eventQ[4];
static double now_ms(void) {

    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

}
//pentla regulacji
void *perform_work(void *arguments) {

    double tim = now_ms();

    PID first = PID(1, 1, 1, 100);
    float out = 0;
    Six_state my;
    Reading br;
    my.zero();
    float u = 0;
    while (1) {
        while (now_ms() - tim < 100);
        tim = now_ms();
        auto b = accelGet(eventQ[0]);
        br.set(b.x, b.y,b.z);
        my.xyz[0](0) += br.val(0) * (0.1);
        out = my.xyz[0](0);
        u = first.run(1 - out);
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "OUTPUT IS %f U IS %f", out,
                            u);

    }

}

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Ini(
        JNIEnv *env,
jobject /* this */) {
    initialization_manager(); //zawsze na początku
   const char *b = getSensorList().c_str();
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "%s", b);
    Matrix<float,4,4> al;
    al = al.Zero();
    al(0,0)= 1;
    al(1,1) = 1;
    al(2,2) = 1;
    al(3,3) = 1;
    Matrix<float,4,1> der;
    der << 2,3,4,0;
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "VECTOR IS %f %f %f", der(0),der(1),der(2));
    der = al*der;
    Matrix<float,2,2> alr;
    alr = alr.Zero();
    alr(0,0) = 1;
    alr(1,1) = 2;

    MatrixXf ab = alr.inverse();
    Matrix<float,4,1> das = der;
    der(0) = das(2);//3
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "VECTOR IS %f %f %f", der(0),der(1),der(2));
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "MATRIX IS %f %f /n %f %f", ab(0,0),ab(0,1),ab(1,0), ab(1,1));

    eventQ[0] = initialization_acceleration(0x01);
eventQ[1] = initialization_gyroscope(0x01);
eventQ[2] = initialization_rotation(0x01);
eventQ[3] = initialization_magnetic(0x01);

}
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Trajectory(
        JNIEnv *env,
jobject /* this */,
jstring fp
) {
const char* SDpath = env->GetStringUTFChars(fp, 0);
std::string temp = SDpath;
temp += "/trajectory.txt";
//Trajectory traj = trajectoryGenerator(temp);
setAttributes(x,0,0,0,1,1);
env->ReleaseStringUTFChars(fp, SDpath);
}

extern struct Reading accel;
extern struct Reading gyro;
extern struct Reading rotation;
extern struct Reading magnetic;
extern struct Six_state dane;

JNIEXPORT jstring JNICALL
Java_com_example_projekt_MainActivity_Update(
        JNIEnv *env,
        jobject /* this */,
        jint i
)
{
    Vector4 rv;
    switch(i){
        case 1:{
            accel.set(accelGet(eventQ[0]));
            (button) ? (rv = accel.getWithOffset()) : (rv = accel.val);
            std::string helper = "Hi your accelerometer reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 2:{
            //odczyt();
            gyro.set(gyroGet(eventQ[1]));
            (button) ? (rv = gyro.getWithOffset()) : (rv = gyro.val);
            std::string helper = "Hi your gyroscope reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 3:{

            rotation.set(rotationGet(eventQ[2]));
            (button) ? (rv = rotation.getWithOffset()) : (rv = rotation.val);
            std::string helper = "Hi your rotation vector reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 4:{
            magnetic.set(magneticGet(eventQ[3]));
            (button) ? (rv = magnetic.getWithOffset()) : (rv = magnetic.val);
            std::string helper = "Hi your magnetic field reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        default:{
            std::string helper = "switch wyszedł poza zakres";
            return env->NewStringUTF(helper.c_str());
        }
    }
}

JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Calibration(
        JNIEnv *env,
        jobject /* this */){
    button=1;
    gyro.cal(1, eventQ[1]);
    accel.cal(2,eventQ[0]);
    rotation.cal(3,eventQ[2]);
    magnetic.cal(4,eventQ[3]);
}
}
