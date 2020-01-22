
#include "sensor.h"
#include"config.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define accelFlag 0x01
#define gyroFlag 0x02
#define magneticFlag 0x04
#define rotationFlag 0x08
//#include <experimental/filesystem>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
char initFlags = 0x00;
ASensorManager *sensorManager;
std::string getSensorList()
{
    ASensorList list;
    std::string a = "Sensor count = ";
   int sensor_count = ASensorManager_getSensorList(sensorManager,&list);
    a+= std::to_string(sensor_count);
    a+= "\r\n";
    for(int i = 0 ; i<sensor_count;i++)
    {
        a += ASensor_getName(list[i]) ;
        a += " sampling rate (hz): ";
        a +=  std::to_string(1000000.0/ASensor_getMinDelay(list[i]));
        a += "\r\n";
    }

return  a;
}
void initialization_manager()
{
    sensorManager = ASensorManager_getInstance();      // referencja do obiektu managera
}

ASensorEventQueue* initialization_acceleration(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS); //zwraca referencje do loopera tego watku lub tworzy nowy gdy go brak
    ALooper_acquire(thr); //zabrania usunieciu loopera, w tym przypadku wydaje sie by nie potrzebne
    const ASensor *acc;
    if (gravitation == 1) {
        acc = ASensorManager_getDefaultSensor(
                sensorManager,
                ASENSOR_TYPE_ACCELEROMETER); //referencja do akcelerometru
    }
    else if (gravitation == 2){
        acc = ASensorManager_getDefaultSensor(
                sensorManager,
                ASENSOR_TYPE_LINEAR_ACCELERATION);
    } //referencja do akcelerometru
    ASensorEventQueue* ev = ASensorManager_createEventQueue(
            sensorManager,
            thr, 3,
            NULL, NULL);//tworzymy kolejke eventow dla naszego looper, 4 argument jest funkcja automatycznie wywolywana jesli looper zbierze event z sensora, tutaj brak jako ze robimy to recznie z javy
    ASensorEventQueue_enableSensor(ev,
                                   acc);
    ASensorEventQueue_setEventRate(ev,
                                   acc,
                                   sampling_rate); //10hz, okres probkowania w mikrosekundach jako argument
    initFlags = initFlags | accelFlag;
    return ev;
}

ASensorEventQueue* initialization_gyroscope(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_GYROSCOPE);
    ASensorEventQueue* ev = ASensorManager_createEventQueue(
            sensorManager,
            thr, 4,
            NULL, NULL);
    ASensorEventQueue_enableSensor( ev,
                                   acc);
    ASensorEventQueue_setEventRate( ev,
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | gyroFlag;
    return ev;
}

ASensorEventQueue* initialization_rotation(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_ROTATION_VECTOR);
    ASensorEventQueue* ev = ASensorManager_createEventQueue(
            sensorManager,
            thr, 5,
            NULL, NULL);
    ASensorEventQueue_enableSensor(ev,
                                   acc);
    ASensorEventQueue_setEventRate(ev,
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | rotationFlag;
    return ev;
}
ASensorEventQueue* initialization_magnetic(char flags)
{
    ALooper *thr = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_acquire(thr);
    const ASensor *acc = ASensorManager_getDefaultSensor(
            sensorManager,
            ASENSOR_TYPE_MAGNETIC_FIELD);
    ASensorEventQueue* ev = ASensorManager_createEventQueue(
            sensorManager,
            thr, 6,
            NULL, NULL);
    ASensorEventQueue_enableSensor(ev,
                                   acc);
    ASensorEventQueue_setEventRate(ev,
                                   acc,
                                   sampling_rate);
    initFlags = initFlags | magneticFlag;
    return ev;
}




Vector4 accelGet(ASensorEventQueue* ev)//accelerator
{
    if(!(accelFlag & initFlags) || ASensorEventQueue_hasEvents(ev) < 1)
        return Vector4(0,0,0);
    ASensorEvent event;
    Vector4 rv;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(ev, &eventTMP, 1) > 0){
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


Vector4 gyroGet(ASensorEventQueue* ev)//gyroscope
{
    if(!(gyroFlag & initFlags))
        return Vector4(0,0,0);
    ASensorEvent event;
    Vector4 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents( ev, &eventTMP, 1) > 0){
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


Vector4 rotationGet(ASensorEventQueue* ev)//rotation
{
    if (!(rotationFlag & initFlags)) {
        Vector4 rv=Vector4(0, 0, 0);
        return rv;
    }
    ASensorEvent event;
    Vector4 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(ev, &eventTMP, 1) > 0){
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

Vector4 magneticGet(ASensorEventQueue* ev)//magnetic
{
    if(!(magneticFlag & initFlags))
        return Vector4(0,0,0);
    ASensorEvent event;
    Vector4 rv;
    ASensorEvent eventTMP;
    while (ASensorEventQueue_getEvents(ev, &eventTMP, 1) > 0){
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
float requestTach(JNIEnv* jenv, jobject jobj) {
    jclass cls = jenv->GetObjectClass(jobj);
    jmethodID mid = jenv->GetMethodID(cls, "tacho", "()F");
   return (jenv)->CallFloatMethod(cls, mid, jobj);

}