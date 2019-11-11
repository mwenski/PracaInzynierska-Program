
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
#include "sensor.h"
#define accelFlag 0x01
//#include <experimental/filesystem>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
#define ourID 3
ASensorEventQueue *eventQ[4];
char initFlags[4] ={0x00,0x00,0x00,0x00};

void initialization_acceleration(char flags)//[0]
{   //if(accelFlag[0] & flags)
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    ASensorManager *sensorManager = ASensorManager_getInstance(); // referencja do obiektu managera
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_LINEAR_ACCELERATION); //referencja do akcelerometru
    eventQ[0] = ASensorManager_createEventQueue(
            sensorManager,
            thr, ourID,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ[0],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[0],
                                   acc,
                                   100000); //10hz, okres probkowania w mikrosekundach jako argument
    initFlags[0] = initFlags[0] | accelFlag;
}

void initialization_gyroscope(char flags)//[1]
{   //if(accelFlag[1] & flags)
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku
    // lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    ASensorManager *sensorManager = ASensorManager_getInstance(); // referencja do obiektu managera
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_GYROSCOPE); //referencja do GYROSCOPU
    eventQ[1] = ASensorManager_createEventQueue(
            sensorManager,
            thr, ourID,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana
    // jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ[1],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[1],
                                   acc,
                                   100000); //10hz, okres probkowania w
    initFlags[1] = initFlags[1] | accelFlag;
}

void initialization_rotation(char flags)//[2]
{   //if(accelFlag[2] & flags)
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku
    // lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    ASensorManager *sensorManager = ASensorManager_getInstance(); // referencja do obiektu managera
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_ROTATION_VECTOR); //referencja do GYROSCOPU
    eventQ[2] = ASensorManager_createEventQueue(
            sensorManager,
            thr, ourID,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana
    // jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ[2],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[2],
                                   acc,
                                   100000); //10hz, okres probkowania w
    initFlags[2] = initFlags[2] | accelFlag;
}
void initialization_magnetic(char flags)//[3]
{   //if(accelFlag[3] & flags)
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku
    // lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    ASensorManager *sensorManager = ASensorManager_getInstance(); // referencja do obiektu managera
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_MAGNETIC_FIELD); //referencja do GYROSCOPU
    eventQ[3] = ASensorManager_createEventQueue(
            sensorManager,
            thr, ourID,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana
    // jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ[3],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[3],
                                   acc,
                                   100000); //10hz, okres probkowania w
    initFlags[3] = initFlags[3] | accelFlag;
}

//========================================================================================

glm::vec3 accelGet1()//accelerator
{
    if(!(accelFlag & initFlags[0]))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ[0], &event, 1) > 0){
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

glm::vec3 accelGet2()//gyroscope
{
    if(!(accelFlag & initFlags[1]))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ[1], &event, 1) > 0){
        //odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "gyroscope: x=%f y=%f z=%f",
                            event.vector.x, event.vector.y,
                            event.vector.z);

        rv.x = event.vector.x*180/M_PI;
        rv.y = event.vector.y*180/M_PI;
        rv.z = event.vector.z*180/M_PI;
    }
    return rv;
}

glm::vec3 accelGet3()//rotation
{
    if(!(accelFlag & initFlags[2]))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ[2], &event, 1) > 0){
        //odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "rotation: x=%f y=%f z=%f",
                            event.vector.x, event.vector.y,
                            event.vector.z);

        rv.x = asin(event.vector.x)*2*180/M_PI;
        rv.y = asin(event.vector.y)*2*180/M_PI;
        rv.z = asin(event.vector.z)*2*180/M_PI;
    }
    return rv;
}

glm::vec3 accelGet4()//magnetic
{
    if(!(accelFlag & initFlags[3]))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ[3], &event, 1) > 0){
        //odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "accelerometer: x=%f y=%f z=%f",
                            event.magnetic.x, event.magnetic.y,
                            event.magnetic.z);

        rv.x = event.magnetic.x;
        rv.y = event.magnetic.y;
        rv.z = event.magnetic.z;
    }
    return rv;
}

//=========================================================================================

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
