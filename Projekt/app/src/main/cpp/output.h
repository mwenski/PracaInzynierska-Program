
#include <jni.h>
#include <stdio.h>
#ifndef PROJEKT_OUTPUT_H
#define PROJEKT_OUTPUT_H



extern "C" {
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Reading(
        JNIEnv *env,
        jobject /* this */,
        jint i);

JNIEXPORT jintArray JNICALL
Java_com_example_projekt_MainActivity_SetSignal(
        JNIEnv *env,
jobject /* this */,
jint n);


}
int ComputeSignal(int mode, float input);




/* class Filter{
    static const int AVERAGE_BUFFER = 10;
    float m_arr[AVERAGE_BUFFER];
    int m_idx = 0;
public:
    Filter();
    ~Filter();
    float append(float val);
    float avg();
};

float ComputeDegrees(float input); */

#endif //PROJEKT_OUTPUT_H
