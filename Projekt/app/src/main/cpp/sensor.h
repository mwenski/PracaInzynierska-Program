//
// Created by John on 19/10/29.
//

#ifndef PROJEKT_SENSOR_H
#define PROJEKT_SENSOR_H
//#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
std::string getSensorList();
void initialization_manager();
void initialization_acceleration(char flags);
void initialization_gyroscope(char flags);
void initialization_rotation(char flags);
void initialization_magnetic(char flags);
glm::vec3 accelGet();
glm::vec3 gyroGet();
glm::vec3 rotationGet();
glm::vec3 magneticGet();
#endif //PROJEKT_SENSOR_H
