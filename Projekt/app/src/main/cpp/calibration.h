//
// Created by John on 19/11/16.
//

#ifndef PROJEKT_CALIBRATION_H
#define PROJEKT_CALIBRATION_H
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
struct Six_state{ //tutaj trzymamy wspolrzedne robota obrotowe, liniowe, i ich pochodne zero je nam zeruje
    glm::vec3 xyz[3]; //3 zmienne liniowe i 2 pochodne
    glm::vec3 rxyz[3];// 3 zmienne obrotowe i 2 ich pochodne
    Six_state();
    void zero();
};
struct Reading{
    glm::vec3 val;  //tutaj trzymamy nasze odczyty z czujnikow
    glm::vec3 offset; // tutaj trzymamy nasza translacje, czyli jak czytam ze telefon jest obrocony o 1/2 pi a chcemy zeby to byl punkt zero to tutaj wrzucamy -1/2 pi
    Reading();
    void zero(); //zerujemy obie wartosci
    void setOffsetAsCurrent(); //wstawiamy wartosc -val jako offset zeby ustawic dany odczyt jako stan 0
    void setOffset(glm::vec3 a); //ustawiamy dany offset
    glm::vec3 getWithOffset();

};
Six_state current;
#endif //PROJEKT_CALIBRATION_H
