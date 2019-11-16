//
// Created by John on 19/11/16.
//
#include"sensor.h"
#include"calibration.h"
Six_state::Six_state() {
zero();
}
void Six_state::zero() {
    for (int i = 0; i < 3; i++) {
        xyz[i] = glm::vec3(0, 0, 0);
        rxyz[i] = glm::vec3(0, 0, 0);
    }
}
void Reading::setOffsetAsCurrent(){
    offset = glm::vec3(-val.x, -val.y, -val.z);
};
void  Reading::setOffset(glm::vec3 a){
    offset = a;
};
glm::vec3 Reading::getWithOffset() {
   return glm::vec3(val.x + offset.x, val.y + offset.y, val.z + offset.z);
}
Reading::Reading()
{
    zero();
}
void Reading::zero()
{
    val = glm::vec3(0,0,0);
    val = glm::vec3(0,0,0);
}