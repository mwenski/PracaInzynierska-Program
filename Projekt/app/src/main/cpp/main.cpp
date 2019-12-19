#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
#include"config.h"
#include"sensor.h"
#include"calibration.h"
#include"trajectory.h"
#include "control.h"
#include <pthread.h>
#include <time.h>
//czas w ms
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
    float u = 0;
    while (1) {
        while (now_ms() - tim < 100);
        tim = now_ms();
        u = first.run(1 - out);
        out = accelGet().x;
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
    Matrice4 al;
    al.cell[0][0] = 1;
    al.cell[1][1] = 1;
    al.cell[2][2] = 1;
    al.cell[3][3] = 1;
    Vector4 der(2,3,4);
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "VECTOR IS %f %f %f", der.x,der.y,der.z);
    der = al*der;
    Vector4 das = der;
    der.x = das.y;//3
    __android_log_print(ANDROID_LOG_INFO, "MainActivity", "VECTOR IS %f %f %f", der.x,der.y,der.z);
initialization_acceleration(0x01);
initialization_gyroscope(0x01);
initialization_rotation(0x01);
initialization_magnetic(0x01);

    pthread_t thread;
    pthread_create(&thread, NULL, perform_work, nullptr);
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
            accel.val=accelGet();
            (button) ? (rv = accel.getWithOffset()) : (rv = accel.val);
            std::string helper = "Hi your accelerometer reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 2:{
            //odczyt();
            gyro.val=gyroGet();
            (button) ? (rv = gyro.getWithOffset()) : (rv = gyro.val);
            std::string helper = "Hi your gyroscope reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 3:{
            // read();

            rotation.val=rotationGet();
            (button) ? (rv = rotation.getWithOffset()) : (rv = rotation.val);
            std::string helper = "Hi your rotation vector reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 4:{
            magnetic.val=magneticGet();
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
   // std::string helper = "switch wyszedł poza zakres";
    //        return env->NewStringUTF(helper.c_str());
}
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Calibration(
        JNIEnv *env,
        jobject /* this */){
    button=1;
    gyro.cal(1);
    accel.cal(2);
    rotation.cal(3);
    magnetic.cal(4);
}







}
