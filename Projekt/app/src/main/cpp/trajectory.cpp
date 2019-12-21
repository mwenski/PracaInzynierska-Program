//
// Created by John on 19/10/29.
//

#include "trajectory.h"
#include <unistd.h>
#include<fstream>
float attributes[3][5] ={0}; //atrybuty dla wielomianu
void setAttributes(AtrHelper xyz, float x4,float x3,float x2,float x1,float x0)//x=0,y=1,z=2
{
        attributes[xyz][0] = x4;
        attributes[xyz][1] = x3;
        attributes[xyz][2] = x2;
        attributes[xyz][3] = x1;
        attributes[xyz][4] = x0;

}
float getPoly(float x4,float x3,float x2,float x1,float x0,float t) //oblicz wielomian z argumentem t x4*t^4...
{
    return (((x4*t)*t + x3*t)*t + x2*t)*t + x1*t + x0;
}
Point getCT(float t)
{
    Point a;
    //kod zwraca obecny punkt trajektorii
    a.x[0] = getPoly(attributes[0][0],attributes[0][1],attributes[0][2],attributes[0][3],attributes[0][4],t);
    a.y[0] = getPoly(attributes[1][0],attributes[1][1],attributes[1][2],attributes[1][3],attributes[1][4],t);
    a.z[0] = getPoly(attributes[2][0],attributes[2][1],attributes[2][2],attributes[2][3],attributes[2][4],t);
    return a;
}