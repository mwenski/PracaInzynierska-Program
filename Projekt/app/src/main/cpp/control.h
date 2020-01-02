//
// Created by John on 19/12/09.
//

#ifndef PROJEKT_CONTROL_H
#define PROJEKT_CONTROL_H

#include "vectors.h"
#include "data.h"
#include "arma/include/armadillo"
using namespace arma;
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
template <int n, int m, int o> //x size, u size, y/z size
struct Kalman{
    arma:: fvec::fixed<n> xhat;
    arma:: fvec::fixed<o> yhat;
    arma:: fvec::fixed<o> zhat;
    arma:: fmat::fixed<n,n> X ;
    arma:: fmat::fixed<n,m> B ;
    arma:: fmat::fixed<o,n> H ;
    arma:: fmat::fixed<n,o> K ;
    arma:: fmat::fixed<o,o> R ;
    arma:: fmat::fixed<n,o> P ;
    arma:: fmat::fixed<o,o> S ;
    arma:: fmat::fixed<n,o> Q ;
    float T;
    void predict(fvec::fixed<m> u);
    void update(fvec::fixed<o> z);
    Kalman();
};
#endif //PROJEKT_CONTROL_H
