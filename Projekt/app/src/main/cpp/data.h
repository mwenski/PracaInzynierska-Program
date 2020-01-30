
#ifndef PROJEKT_CALIBRATION_H
#define PROJEKT_CALIBRATION_H

#include "vectors.h"
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include "Eigen/Eigen/Dense"
using namespace Eigen;
struct Six_state{ //tutaj trzymamy wspolrzedne robota obrotowe, liniowe, i ich pochodne zero je nam zeruje
    Matrix<float,4,1> xyz[3]; //3 zmienne liniowe i 2 pochodne
    Matrix<float,4,1> rxyz[3];// 3 zmienne obrotowe i 2 ich pochodne
    void transformFrame(Matrix<float,4,4>);
    Six_state();
    void zero();
};
struct Reading{
    Matrix<float,4,1> val;  //tutaj trzymamy nasze odczyty z czujnikow
    Matrix<float,4,1> offset; // tutaj trzymamy nasza translacje, czyli jak czytam ze telefon jest obrocony o 1/2 pi a chcemy zeby to byl punkt zero to tutaj wrzucamy -1/2 pi
    Reading();
    void zero(); //zerujemy obie wartosci
    void set(float a, float b, float c);
    void set(Vector4 a);
    void set(Matrix<float,4,1> a);
    void setOffsetAsCurrent(); //wstawiamy wartosc -val jako offset zeby ustawic dany odczyt jako stan 0
    void setOffset(Vector4 a); //ustawiamy dany offset
    void setOffset(Matrix<float,4,1> a);
    Matrix<float,4,1> getWithOffset();
    void cal(int a,ASensorEventQueue* ev);

};

Matrix<float,4,4> eulerZYZ(float a, float b, float c);
Matrix<float,4,4> rotationX(float a);
Matrix<float,4,4> rotationY(float a);
Matrix<float,4,4> rotationZ(float a);
Matrix<float,4,4> translation(float x, float y,float z);
Matrix<float,4,4> translation(Matrix<float,3,1> a);
#endif //PROJEKT_DATA_H