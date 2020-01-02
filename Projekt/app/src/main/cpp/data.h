//
// Created by John on 19/11/16.
//

#ifndef PROJEKT_DATA_H
#define PROJEKT_DATA_H
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include "config.h"
#include "vectors.h"
struct Six_state{ //tutaj trzymamy wspolrzedne robota obrotowe, liniowe, i ich pochodne zero je nam zeruje
    Vector4 xyz[3]; //3 zmienne liniowe i 2 pochodne
    Vector4 rxyz[3];// 3 zmienne obrotowe i 2 ich pochodne
    Six_state();
    void zero();
};
struct Reading{
    Vector4 val;  //tutaj trzymamy nasze odczyty z czujnikow
    Vector4 offset; // tutaj trzymamy nasza translacje, czyli jak czytam ze telefon jest obrocony o 1/2 pi a chcemy zeby to byl punkt zero to tutaj wrzucamy -1/2 pi
    Reading();
    void zero(); //zerujemy obie wartosci
    void set(float a, float b, float c);
    void set(Vector4 a);
    void setOffsetAsCurrent(); //wstawiamy wartosc -val jako offset zeby ustawic dany odczyt jako stan 0
    void setOffset(Vector4 a); //ustawiamy dany offset
    Vector4 getWithOffset();
    void cal(int a,ASensorEventQueue* ev);

};
#endif //PROJEKT_DATA_H
