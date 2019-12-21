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