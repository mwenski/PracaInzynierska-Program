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
template<int n, int m, int o>
LObserver<n,m,o>::LObserver(arma:: fmat::fixed<n,o> l, arma:: fmat::fixed<n,n> x, arma:: fmat::fixed<n,m> b, arma:: fmat::fixed<o,n> c, arma:: fmat::fixed<o,m> d, float t)
{
    L=l;
    X=x;
    B=b;
    C=c;
    D=d;
    T=t;
    xhat.fill(0);
    yhat.fill(0);
}
template<int n, int m, int o>
void LObserver<n,m,o>::predict(arma::fvec::fixed<m> u,arma::fvec::fixed<o> y) //funkcja obserwatora Luenberga która oblicza następny krok metoda eulera w wstecz
{
    yhat = C*xhat + D*u;
    xhat += T*(X*xhat + B*u + L*(y-yhat));

}
template<int n, int m, int o>
arma::fvec::fixed<n> LObserver<n,m,o>::getState()
{
    return xhat;
}
template<int n, int m, int o>
void Kalman<n,m,o>::predict(fvec::fixed<m> u) //provide inputs to get kalman prediction for the next timestep
{
 xhat = X*xhat + B*u;
 P = X*P*X.t()  + Q; // X.t() is transpose of X
}
template<int n, int m, int o>
void Kalman<n,m,o>::update(fvec::fixed<o> z) //provide a measurement z to update kalman filter
{
    yhat = zhat - H*xhat;
    S = H*P*H.t() + R;
    K = P*H*S.i(); //S.i() returns inverse of S
    xhat = xhat + K*yhat;
    P = (eye(n,n) - K*H)*P;
    yhat = z - H*xhat;

}
template<int n, int m, int o>
Kalman<n,m,o>::Kalman()
{
    xhat.fill(0.0);
    yhat.fill(0.0);
    zhat.fill(0.0);
}