
#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#include<vector>
#include<fstream>
#include <camera/NdkCameraCaptureSession.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraMetadataTags.h>
#include <camera/NdkCameraWindowType.h>
#include <camera/NdkCaptureRequest.h>
#include <camera/NdkCameraManager.h>
//#include <experimental/filesystem>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
#define ourID 3
ASensorEventQueue* eventQ;
struct Point{
float x[3],y[3],z[3];
};
struct Trajectory{
std::vector<Point> coordinates;
float step;
};

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


}
JNIEXPORT jstring JNICALL
        Java_com_example_projekt_MainActivity_Update(
                JNIEnv *env,
                jobject /* this */
                )
{

        return env->NewStringUTF("Hi");

}
}
