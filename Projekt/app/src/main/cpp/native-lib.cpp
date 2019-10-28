
#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#include<vector>
#include<fstream>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
#include <camera/NdkCameraCaptureSession.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraMetadataTags.h>
#include <camera/NdkCameraWindowType.h>
#include <camera/NdkCaptureRequest.h>
#include <camera/NdkCameraManager.h>
#define accelFlag 0x01
//#include <experimental/filesystem>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
#define ourID 3
ASensorEventQueue* eventQ;
char initFlags = 0x00;
struct Point{
float x[3],y[3],z[3];
};

struct Trajectory{
std::vector<Point> coordinates;
float step;
};
void initialization(char flags)
{   //if(accelFlag & flags)
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    ASensorManager *sensorManager = ASensorManager_getInstance(); // referencja do obiektu managera
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_ACCELEROMETER); //referencja do akcelerometru
    eventQ = ASensorManager_createEventQueue(
            sensorManager,
            thr, ourID,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ,
                                   acc);
    ASensorEventQueue_setEventRate(eventQ,
                                   acc,
                                   100000); //10hz, okres probkowania w mikrosekundach jako argument
    initFlags = initFlags | accelFlag;
}
glm::vec3 accelGet() {
    if(!(accelFlag & initFlags))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ, &event, 1) > 0){
     //odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "accelerometer: x=%f y=%f z=%f",
                            event.acceleration.x, event.acceleration.y,
                            event.acceleration.z);

    rv.x = event.acceleration.x;
    rv.y = event.acceleration.y;
    rv.z = event.acceleration.z;
    }
    return rv;


}
Trajectory trajectoryGenerator(std::string fileName)
{
    std::ifstream fr;
    fr.open(fileName);
    Trajectory traj;
    if(fr.failbit != std::ios_base::goodbit)
           return traj;
    std::string p;
    fr>>p; //first line is delta T for whole trajector;
    traj.step= atof(p.c_str());
    while(!fr.eof())
    {
        fr>>p;
        Point a;
        int comf=p.find(',');
        int coms = p.substr(comf).find(',');
        a.x[0]=atof(p.substr(0,comf).c_str());
        a.y[0]=atof(p.substr(comf,comf-coms).c_str());
        a.z[0]=atof(p.substr(coms).c_str());
        traj.coordinates.push_back(a);
    }

    for(int i = 0;i<traj.coordinates.size();i++) { //filling first and second delta
        if(i!=0) {
            traj.coordinates[i-1].x[1] = traj.coordinates[i].y[0] - traj.coordinates[i-1].y[0];
            traj.coordinates[i-1].y[1] = traj.coordinates[i].y[0] - traj.coordinates[i-1].y[0];
            traj.coordinates[i-1].z[1] = traj.coordinates[i].z[0] - traj.coordinates[i-1].z[0];
        }
        if(i!=1)
        {
            traj.coordinates[i-2].x[2] = traj.coordinates[i-1].y[1] - traj.coordinates[i-2].y[1];
            traj.coordinates[i-2].y[2] = traj.coordinates[i-1].y[1] - traj.coordinates[i-2].y[1];
            traj.coordinates[i-2].z[2] = traj.coordinates[i-1].z[1] - traj.coordinates[i-2].z[1];
        }
    }
    return traj;
}
 void onDisconnected(void* context, ACameraDevice* device)
{
    // ...
}

 void onError(void* context, ACameraDevice* device, int error)
{
    // ...
}

 ACameraDevice_stateCallbacks cameraDeviceCallbacks = {
        .context = nullptr,
        .onDisconnected = onDisconnected,
        .onError = onError,
};


extern "C" {
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Ini(
        JNIEnv *env,
        jobject /* this */) {
initialization(0x01);
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
    Trajectory traj = trajectoryGenerator(temp);
    env->ReleaseStringUTFChars(fp, SDpath);
}
JNIEXPORT jstring JNICALL
        Java_com_example_projekt_MainActivity_Update(
                JNIEnv *env,
                jobject /* this */
                )
{
        glm::vec3 rv = accelGet();
        std::string helper = "Hi your accelerometer reads (x,y,z) "+ std::to_string(rv.x) + " " + std::to_string(rv.y) + " "+ std::to_string(rv.z);
        return env->NewStringUTF(helper.c_str());

}

}
