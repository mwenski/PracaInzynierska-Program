
#ifndef PROJEKT_SENSOR_H
#define PROJEKT_SENSOR_H
#include "vectors.h"
#include<string>

std::string getSensorList();
void initialization_manager();
void initialization_acceleration(char flags);
void initialization_gyroscope(char flags);
void initialization_rotation(char flags);
void initialization_magnetic(char flags);
Vector4 accelGet();
Vector4 gyroGet();
Vector4 rotationGet();
Vector4 magneticGet();
#endif //PROJEKT_SENSOR_H
