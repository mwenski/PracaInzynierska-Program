//
// Created by John on 19/11/16.
//
#include"sensor.h"
#include"calibration.h"
#include <unistd.h>
#include "config.h"

#include<math.h>

Six_state::Six_state() {
zero();
}
void Six_state::zero() {
    for (int i = 0; i < 3; i++) {
        xyz[i] = Vector4(0, 0, 0);
        rxyz[i] = Vector4(0, 0, 0);
    }
}
void Reading::setOffsetAsCurrent(){
    offset = Vector4(-val.x, -val.y, -val.z);
};
void  Reading::setOffset(Vector4 a){
    offset = a;
};
Vector4 Reading::getWithOffset() {
   return Vector4(val.x + offset.x, val.y + offset.y, val.z + offset.z);
}
Reading::Reading()
{
    zero();
}
void Reading::set(float a, float b, float c)
{
    val = Vector4(a,b,c);
}
void Reading::set(Vector4 a)
{
    val = a;
}
void Reading::zero()
{
    val = Vector4(0,0,0);
    offset = Vector4(0,0,0);
}


Reading rotation;
Reading accel;
Reading gyro;
Reading magnetic;
Six_state dane;
void Reading::cal(int a){

    int num_samples = 5;
    Vector4 rv[num_samples];
    Vector4 sr(0,0,0);

    //srednia
    for (int i = 0; i < num_samples; i++) {

        //__android_log_print(ANDROID_LOG_INFO, "MainActivity", "Czytamy");
        usleep(2.05*sampling_rate);
        if(a==1)
            rv[i] = accelGet();
        if(a==2)
            rv[i] = gyroGet();
        if(a==3)
            rv[i] = rotationGet();
        if(a==4)
            rv[i] = magneticGet();
    }
    for (int i = 0; i < num_samples; i++) {
         sr = sr + rv[i];
    }
    sr= sr/num_samples;

    //odchylenie standardowe
    Vector4 odchylenie= Vector4(0,0,0);
    Vector4 zmienna= Vector4(0,0,0);
    for (int i = 0; i < num_samples; i++) {
        zmienna.x += pow(rv[i].x - sr.x, 2);
        zmienna.y += pow(rv[i].y - sr.y, 2);
        zmienna.z += pow(rv[i].z - sr.z, 2);
    }

    odchylenie.x = -sqrt(zmienna.x / num_samples)- sr.x;
    odchylenie.y = -sqrt(zmienna.y / num_samples)- sr.y;
    odchylenie.z = -sqrt(zmienna.z / num_samples)- sr.z;/////- sr.z
    setOffset(odchylenie);
}



