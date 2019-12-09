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
void PID::setParams(float p, float i, float d, float t)
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
    memi+= in*T;
    ret = memi*Ki + in*Kp + ((in-memd)/T)*Kd;
    memd = in;
    return ret;
}
