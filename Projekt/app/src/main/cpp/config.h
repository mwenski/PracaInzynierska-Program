
#include <jni.h>
#ifndef PROJEKT_CONFIG_H
#define PROJEKT_CONFIG_H
extern int config_variable_a;
extern int sampling_rate;
extern bool button;
extern int gravitation;
extern float Tp;

extern "C" JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_load(
        JNIEnv *env,
        jobject /* this */,
        jint a);

#endif //PROJEKT_CONFIG_H
