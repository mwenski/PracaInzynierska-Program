//
// Created by John on 19/12/09.
//

#ifndef PROJEKT_CONTROL_H
#define PROJEKT_CONTROL_H

#include "vectors.h"
<<<<<<< Updated upstream
#include "calibration.h"
#include "arma/include/armadillo"
=======
#include "data.h"
//#include "arma/include/armadillo"
#include "Eigen/Eigen/Dense"
using namespace Eigen;
>>>>>>> Stashed changes
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
    Matrix<float,n,1> xhat;
    Matrix<float,o,1> yhat;
    Matrix<float,n,o> L ;
    Matrix<float,n,n> X ;
    Matrix<float,n,m> B ;
    Matrix<float,o,n> C ;
    Matrix<float,o,m> D ;
    float T;
public:
    void predict(Matrix<float,m,1> u,Matrix<float,o,1> y);
    LObserver(Matrix<float,n,o> l, Matrix<float,n,n> x, Matrix<float,n,m> b, Matrix<float,o,n> c, Matrix<float,o,m> d, float t);
    Matrix<float,n,1> getState();

};
<<<<<<< Updated upstream
=======
template <int n, int m, int o> //x size, u size, y/z size
struct Kalman{
    Matrix<float,n,1> xhat;
    Matrix<float,o,1> yhat;
    Matrix<float,o,1> zhat;
    Matrix<float,n,n> X ;
    Matrix<float,n,m> B ;
    Matrix<float,o,n> H ;
    Matrix<float,n,o> K ;
    Matrix<float,o,o> R ;
    Matrix<float,n,o> P ;
    Matrix<float,o,o> S ;
    Matrix<float,n,o> Q ;
    float T;
    void predict(Matrix<float,m,1> u);
    void update(Matrix<float,o,1> z);
    Kalman();
};
>>>>>>> Stashed changes
#endif //PROJEKT_CONTROL_H
