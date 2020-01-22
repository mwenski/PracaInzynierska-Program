//
// Created by John on 19/10/29.
//
#include"config.h"
int config_variable_a = 12;
int sampling_rate = 100000; //w mikrosekundach
bool button = 0;
int gravitation;
float Tp = 100.0;

extern "C" JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_load(
        JNIEnv *env,
jobject /* this */,
jint a){
gravitation=a;
}

