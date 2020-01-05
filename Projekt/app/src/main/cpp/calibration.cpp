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
        xyz[i] = {0, 0, 0, 0};
        rxyz[i] = {0, 0, 0, 0};
    }
}
void Six_state::transformFrame(Matrix<float,4,4> A) //transformuj wspolrzedne do danego ukladu odniesienia
{

        xyz[0] = A*xyz[0];
        xyz[1] = A*xyz[1];
        xyz[2] = A*xyz[2];
        xyz[1](3) = 0;
        xyz[2](3) = 0;

};


void Reading::setOffsetAsCurrent(){
    offset <<-val(0), -val(1), -val(2),1;
};
void  Reading::setOffset(Vector4 a){
    offset(0) = a.val[0];
    offset(1) = a.val[1];
    offset(2) = a.val[2];
    offset(3) = a.val[3];
};
void Reading::setOffset(Matrix<float,4,1> a)
{
    offset = a;
}
Matrix<float,4,1> Reading::getWithOffset() {
   return val + offset;
}
Reading::Reading()
{
    zero();
}
void Reading::set(float a, float b, float c)
{
    val = {a,b,c,1};
}
void Reading::set(Vector4 a)
{
    val(0) = a.val[0];
    val(1) = a.val[1];
    val(2) = a.val[2];
    val(3) = a.val[3];
}
void Reading::set(Matrix<float,4,1> a)
{
val = a;
}

void Reading::zero()
{
    val = val.Zero();
    offset = offset.Zero();
}
Matrix<float,4,4> eulerZYZ(float a, float b, float c)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    rv(0,0) = cos(a)*cos(b)*cos(c) - sin(a)*sin(c);
    rv(0,1) = -cos(c)*sin(a) - cos(a)*cos(b)*sin(c);
    rv(0,2) = cos(a)*sin(b);
    rv(1,0) = cos(a)*sin(c)+cos(b)*cos(c)*sin(a);
    rv(1,1) = cos(a)*cos(c)-cos(b)*sin(a)*sin(b);
    rv(1,2) = sin(a)*sin(b);
    rv(2,0) = -cos(c)*sin(b);
    rv(2,1) = sin(b)*sin(c);
    rv(2,2) = cos(b);
    rv(3,3) = 1;
    return rv;

}
Matrix<float,4,4> rotationX(float a)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    rv(0,0) = 1;
    rv(1,1) = cos(a);
    rv(1,2) = -sin(a);
    rv(2,1) = sin(a);
    rv(2,2) = cos(a);
    rv(3,3) = 1;
    return rv;
}
Matrix<float,4,4> rotationY(float a)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    rv(0,0) = cos(a);
    rv(1,1) = 1;
    rv(0,2) = sin(a);
    rv(2,2) = cos(a);
    rv(2,0) = -sin(a);
    rv(3,3) = 1;
    return rv;
}
Matrix<float,4,4> rotationZ(float a)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    rv(0,0) = cos(a);
    rv(1,1) = cos(a);
    rv(0,1) = -sin(a);
    rv(1,0) = sin(a);
    rv(2,2) = 1;
    rv(3,3) = 1;
    return rv;
}
Matrix<float,4,4> translation(float x, float y,float z)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    rv(0,3) = x;
    rv(1,3) = y;
    rv(2,3) = z;
    rv(3,3) = 1;
    return rv;
}

Matrix<float,4,4> translation(Matrix<float,3,1> a)
{
    Matrix<float,4,4> rv;
    rv = rv.Zero();
    if(a.size()<3) return rv;
    rv(0,3) = a(0);
    rv(1,3) = a(1);
    rv(2,3) = a(2);
    rv(3,3) = 1;
    return rv;
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


        sr.x += rv[i].x;
        sr.y += rv[i].y;
        sr.z += rv[i].z;
    }
    sr.x = sr.x / num_samples;
    sr.y = sr.y / num_samples;
    sr.z = sr.z / num_samples;
    //odchylenie standardowe
    Vector4 odchylenie= Vector4(0,0,0);
    Vector4 zmienna= Vector4(0,0,0);
    for (int i = 0; i < num_samples; i++) {
        zmienna.x += pow(rv[i].x - sr.x, 2);
        zmienna.y += pow(rv[i].y - sr.y, 2);
        zmienna.z += pow(rv[i].z - sr.z, 2);
    }
    odchylenie.x = -sqrtf(zmienna.x / num_samples)- sr.x;
    odchylenie.y = -sqrtf(zmienna.y / num_samples)- sr.y;
    odchylenie.z = -sqrtf(zmienna.z / num_samples)- sr.z;/////- sr.z
    setOffset(odchylenie);
}


