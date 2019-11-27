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
        xyz[i] = Vector4(0, 0, 0);
        rxyz[i] = Vector4(0, 0, 0);
    }
}
void Reading::setOffsetAsCurrent(){
    offset = Vector4(-val.x, -val.y, -val.z);
};
void  Reading::setOffset(Vector4 a){
    offset = a;
};
Vector4 Reading::getWithOffset() {
   return Vector4(val.x + offset.x, val.y + offset.y, val.z + offset.z);
}
Reading::Reading()
{
    zero();
}
void Reading::zero()
{
    val = Vector4(0,0,0);
    val = Vector4(0,0,0);
}