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
    double T;
    float memi = 0;
    float memd = 0;
public:
    PID(float p, float i, float d, float t);
    void setParams(float p, float i, float d, double t);
    void clearMemory();
    float run(float in);
};
class LObserver{ //obserwator Luenberga 1 wymiarowy
    float xhat;
    float yhat;
    float L;
    float X;
    float B;
    float C;
    float D;
    void predict(float u, float y);
    LObserver(float l, float x, float b, float c, float d);

};
#endif //PROJEKT_CONTROL_H
