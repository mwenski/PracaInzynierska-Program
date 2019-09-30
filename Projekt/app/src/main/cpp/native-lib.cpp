#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MainActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MainActivity", __VA_ARGS__))
#define ourID 3
ASensorEventQueue* eventQ;

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Ini(
        JNIEnv *env,
        jobject /* this */) {
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

}
JNIEXPORT jstring JNICALL
        Java_com_example_projekt_MainActivity_Update(
                JNIEnv *env,
                jobject /* this */
                )
{
   // ALooper_pollAll(0, NULL, NULL, NULL);
    ASensorEvent event;
    std::string hello;
    //odczytuje wszystkie zebrane eventy i bierze najnowszego na wyjscie (ostatnieg)
    while (ASensorEventQueue_getEvents(eventQ, &event, 1) > 0){ //odbieramy event
        __android_log_print(ANDROID_LOG_INFO, "MainActivity", "accelerometer: x=%f y=%f z=%f",
                            event.acceleration.x, event.acceleration.y,
                            event.acceleration.z);
        hello = "accelerometer: x=" + std::to_string(event.acceleration.x) + ", y=" +
                            std::to_string(event.acceleration.y) + ", z=" +
                            std::to_string(event.acceleration.z);}
        return env->NewStringUTF(hello.c_str());

}
}
