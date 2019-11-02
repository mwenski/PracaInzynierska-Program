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
void initialization_acceleration(char flags);
void initialization_gyroscope(char flags);
void initialization_rotation(char flags);
void initialization_magnetic(char flags);
glm::vec3 accelGet1();
glm::vec3 accelGet2();
glm::vec3 accelGet3();
glm::vec3 accelGet4();
#endif //PROJEKT_SENSOR_H
