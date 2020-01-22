
#ifndef PROJEKT_SENSOR_H
#define PROJEKT_SENSOR_H

//#include <GLES2/gl2.h>
#include <JNI.h>
#include "vectors.h"
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <camera/NdkCameraCaptureSession.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraMetadataTags.h>
#include <camera/NdkCameraWindowType.h>
#include <camera/NdkCaptureRequest.h>
#include <camera/NdkCameraManager.h>
#include <string>
#include <unistd.h>
#include<vector>
#include<fstream>

std::string getSensorList();
void initialization_manager();
ASensorEventQueue* initialization_acceleration(char flags);
ASensorEventQueue* initialization_gyroscope(char flags);
ASensorEventQueue* initialization_rotation(char flags);
ASensorEventQueue* initialization_magnetic(char flags);
Vector4 accelGet(ASensorEventQueue* ev);
Vector4 gyroGet(ASensorEventQueue* ev);
Vector4 rotationGet(ASensorEventQueue* ev);
Vector4 magneticGet(ASensorEventQueue* ev);
float requestTach(JNIEnv* jenv, jobject jobj);

#endif //PROJEKT_SENSOR_H
