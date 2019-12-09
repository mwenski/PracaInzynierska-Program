//
// Created by John on 19/12/09.
//

#ifndef PROJEKT_CONTROL_H
#define PROJEKT_CONTROL_H

#include "vectors.h"
#include "calibration.h"
class PID{ // Klasa regulatora PID o wzorze Kp * e(t) + Ki * (sum 0 -> t e(t)) + Kd * (e(t) - e(t-1))/T
    float Kp;
    float Ki;
    float Kd;
    float T;
    float memi = 0;
    float memd = 0;
public:
    PID(float p, float i, float d, float t);
    void setParams(float p, float i, float d, float t);
    void clearMemory();
    float run(float in);
};

#endif //PROJEKT_CONTROL_H
