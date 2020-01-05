//
// Created by John on 19/11/27.
//

#ifndef PROJEKT_VECTORS_H
#define PROJEKT_VECTORS_H
#include "Eigen/Eigen/Dense"
using namespace Eigen;
struct Vector4{
    float val[4];
    float& x = val[0];
    float& y = val[1];
    float& z = val[2];
    float& w = val[3];
    Vector4 operator+(Vector4 &a);
    Vector4& operator=(const Vector4 &other);
    Vector4& operator=(const Matrix<float,4,1> &other);
    Vector4(float x,float y,float z);
    Vector4();

};

struct Matrice4
{
    float cell[4][4] = {0};
    Matrice4 operator*(Matrice4 &b);
    Vector4 operator*(Vector4 &b);
    Matrice4 transpose();
};
#endif //PROJEKT_VECTORS_H
