//
// Created by John on 19/10/29.
//

#ifndef PROJEKT_TRAJECTORY_H
#define PROJEKT_TRAJECTORY_H
#include<vector>
#include<string>
struct Point{
    float x[3],y[3],z[3];
};
enum AtrHelper{x=0,y=1,z=2};
void setAttributes(AtrHelper xyz, float x4,float x3,float x2,float x1,float x0);
Point getCT(float t);
float getPoly(float x4,float x3,float x2,float x1,float x0,float t);
#endif //PROJEKT_TRAJECTORY_H
