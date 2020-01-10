//
// Created by John on 19/12/09.
//

#ifndef PROJEKT_CONTROL_H
#define PROJEKT_CONTROL_H

#include "vectors.h"
#include "calibration.h"
#include "arma/include/armadillo"
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
template <int n, int m, int o> //X jest nxn, B jest nxm C jest oxn a D jest oxm
class LObserver{ //obserwator Luenberga 1 wymiarowy
    arma:: fvec::fixed<n> xhat;
    arma:: fvec::fixed<o> yhat;
    arma:: fmat::fixed<n,o> L ;
    arma:: fmat::fixed<n,n> X ;
    arma:: fmat::fixed<n,m> B ;
    arma:: fmat::fixed<o,n> C ;
    arma:: fmat::fixed<o,m> D ;
    float T;
public:
    void predict(arma::fvec::fixed<m> u,arma::fvec::fixed<o> y);
    LObserver(arma:: fmat::fixed<n,o> l, arma:: fmat::fixed<n,n> x, arma:: fmat::fixed<n,m> b, arma:: fmat::fixed<o,n> c, arma:: fmat::fixed<o,m> d, float t);
    arma::fvec::fixed<n> getState();

};
#endif //PROJEKT_CONTROL_H
