
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
ASensorEventQueue* eventQ;
char initFlags = 0x00;

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
glm::vec3 accelGet()
{
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
