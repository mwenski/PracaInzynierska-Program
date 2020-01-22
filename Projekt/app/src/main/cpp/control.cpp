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
    memd = 0;
    memi = 0;
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
template<int n, int m, int o>
LObserver<n,m,o>::LObserver(Matrix<float,n,o> l, Matrix<float,n,n> x, Matrix<float,n,m> b, Matrix<float,o,n> c, Matrix<float,o,m> d, float t)
{
    L=l;
    X=x;
    B=b;
    C=c;
    D=d;
    T=t;
    xhat = xhat.Zero() ;
    yhat = yhat.Zero();
}
template<int n, int m, int o>
void LObserver<n,m,o>::predict(Matrix<float,m,1> u,Matrix<float,o,1> y) //funkcja obserwatora Luenberga która oblicza następny krok metoda eulera w wstecz
{
    yhat = C*xhat + D*u;
    xhat += T*(X*xhat + B*u + L*(y - yhat));

}
template<int n, int m, int o>
Matrix<float,n,1> LObserver<n,m,o>::getState()
{
    return xhat;
}
template<int n, int m, int o>
void Kalman<n,m,o>::predict(Matrix<float,m,1> u) //provide inputs to get kalman prediction for the next timestep
{
 xhat = X*xhat + B*u;
 P = X*P*X.transpose()  + Q; // X.t() is transpose of X
}
template<int n, int m, int o>
void Kalman<n,m,o>::update(Matrix<float,o,1> z) //provide a measurement z to update kalman filter
{   Matrix<float, n,n> helper;
helper = helper.setIdentity();
    MatrixXf helper2 = S.inverse();
    yhat = zhat - H*xhat;
    S = H*P*H.transpose() + R;
    K = P*H*helper2; //S.i() returns inverse of S
    xhat = xhat + K*yhat;
    P = (helper - K*H)*P;
    yhat = z - H*xhat;

}
template<int n, int m, int o>
Kalman<n,m,o>::Kalman()
{
    xhat = xhat.Zero();
    yhat = yhat.Zero();
    zhat = zhat.Zero();
}