//
// Created by John on 19/12/09.
//

#include "control.h"
PID::PID(float p, float i, float d, float t)
{
    Kp=p;
    Ki=i;
    Kd=d;
    T=t;
}
void PID::setParams(float p, float i, float d, double t)
{
    Kp=p;
    Ki=i;
    Kd=d;
    T=t;
}
void PID::clearMemory()
{
    memi=0;
    memd=0;
}
float PID::run(float in){

    float ret;
    memi+= in*(T/1000);
    ret = memi*Ki + in*Kp + ((in-memd)/(T/1000))*Kd;
    memd = in;
    return ret;
}

LObserver::LObserver(float l, float x, float b, float c, float d)
{
    L=l;
    X=x;
    B=b;
    C=c;
    D=d;
}
void LObserver::predict(float u, float y)
{
    xhat = X*xhat + B*u + L*(y-yhat);
    yhat = C*xhat + D*u;
}
