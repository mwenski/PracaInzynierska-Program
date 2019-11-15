
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
#include <cmath>
#include"config.h"
#define accelFlag 0x01
#define gyroFlag 0x02
#define magneticFlag 0x03
#define rotationFlag 0x04
//#include <experimental/filesystem>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
ASensorEventQueue *eventQ[4];
char initFlags = 0x00;
ASensorManager *sensorManager;

void initialization_manager()
{
    sensorManager = ASensorManager_getInstance();      // referencja do obiektu managera
}
void initialization_acceleration(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_LINEAR_ACCELERATION); //referencja do akcelerometru
    eventQ[0] = ASensorManager_createEventQueue(
            sensorManager,
            thr, 3,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(eventQ[0],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[0],
                                   acc,
                                   sampling_rate); //10hz, okres probkowania w mikrosekundach jako argument
    initFlags = initFlags | accelFlag;
}

void initialization_gyroscope(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_GYROSCOPE);
    eventQ[1] = ASensorManager_createEventQueue(
            sensorManager,
            thr, 4,
            NULL, NULL);
    ASensorEventQueue_enableSensor(eventQ[1],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[1],
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | gyroFlag;
}

void initialization_rotation(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_ROTATION_VECTOR);
    eventQ[2] = ASensorManager_createEventQueue(
            sensorManager,
            thr, 5,
            NULL, NULL);
    ASensorEventQueue_enableSensor(eventQ[2],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[2],
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | rotationFlag;
}
void initialization_magnetic(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_MAGNETIC_FIELD);
    eventQ[3] = ASensorManager_createEventQueue(
            sensorManager,
            thr, 6,
            NULL, NULL);
    ASensorEventQueue_enableSensor(eventQ[3],
                                   acc);
    ASensorEventQueue_setEventRate(eventQ[3],
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | magneticFlag;
}

//========================================================================================

glm::vec3 accelGet()//accelerator
{
    if(!(accelFlag & initFlags) || ASensorEventQueue_hasEvents(eventQ[0]) < 1)
        return glm::vec3(0,0,0);
    ASensorEvent event;

    glm::vec3 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(eventQ[0], &eventTMP, 1) > 0){
        event = eventTMP;
    }//odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "accelerometer: x=%f y=%f z=%f",
                            event.acceleration.x, event.acceleration.y,
                            event.acceleration.z); 
        rv.x = event.acceleration.x;
        rv.y = event.acceleration.y;
        rv.z = event.acceleration.z;

    return rv;
}

glm::vec3 gyroGet()//gyroscope
{
    if(!(gyroFlag & initFlags))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(eventQ[1], &eventTMP, 1) > 0){
        event = eventTMP;
    }
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "gyroscope: x=%f y=%f z=%f",
                            event.vector.x, event.vector.y,
                            event.vector.z);

        rv.x = event.vector.x*180/M_PI;
        rv.y = event.vector.y*180/M_PI;
        rv.z = event.vector.z*180/M_PI;

    return rv;
}

glm::vec3 rotationGet()//rotation
{
    if(!(rotationFlag & initFlags))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(eventQ[2], &eventTMP, 1) > 0){
        event = eventTMP;
    }
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "rotation: x=%f y=%f z=%f",
                            event.vector.x, event.vector.y,
                            event.vector.z);

        rv.x = asin(event.vector.x)*2*180/M_PI;
        rv.y = asin(event.vector.y)*2*180/M_PI;
        rv.z = asin(event.vector.z)*2*180/M_PI;

    return rv;
}

glm::vec3 magneticGet()//magnetic
{
    if(!(magneticFlag & initFlags))
        return glm::vec3(0,0,0);
    ASensorEvent event;
    glm::vec3 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(eventQ[3], &eventTMP, 1) > 0){
        event = eventTMP;
    }
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "accelerometer: x=%f y=%f z=%f",
                            event.magnetic.x, event.magnetic.y,
                           event.magnetic.z);

        rv.x = event.magnetic.x;
        rv.y = event.magnetic.y;
        rv.z = event.magnetic.z;

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
