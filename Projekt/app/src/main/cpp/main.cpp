#include <jni.h>
#include <string>
#include <android/sensor.h>
#include <android/log.h>
#include <android/looper.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
#include"config.h"
#include"sensor.h"
#include"trajectory.h"
extern "C" {
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Ini(
        JNIEnv *env,
jobject /* this */) {
    initialization_manager(); //zawsze na początku
initialization_acceleration(0x01);
initialization_gyroscope(0x01);
initialization_rotation(0x01);
initialization_magnetic(0x01);
}
JNIEXPORT void JNICALL
Java_com_example_projekt_MainActivity_Trajectory(
        JNIEnv *env,
jobject /* this */,
jstring fp
) {
const char* SDpath = env->GetStringUTFChars(fp, 0);
std::string temp = SDpath;
temp += "/trajectory.txt";
Trajectory traj = trajectoryGenerator(temp);
env->ReleaseStringUTFChars(fp, SDpath);
}
JNIEXPORT jstring JNICALL
Java_com_example_projekt_MainActivity_Update(
        JNIEnv *env,
        jobject /* this */,
        jint i
)
{
    switch(i){
        case 1:{
        glm::vec3 rv = accelGet();
        std::string helper = "Hi your accelerometer reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                             + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 2:{
            glm::vec3 rv = gyroGet();
            std::string helper = "Hi your gyroscope reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 3:{
            glm::vec3 rv = rotationGet();
            std::string helper = "Hi your rotation vector reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        case 4:{
            glm::vec3 rv = magneticGet();
            std::string helper = "Hi your magnetic field reads (x,y,z) X: " + std::to_string(rv.x) + " Y: "
                                 + std::to_string(rv.y) + " Z: " + std::to_string(rv.z);
            return env->NewStringUTF(helper.c_str());
        }
        default:{
            std::string helper = "switch wyszedł poza zakres";
            return env->NewStringUTF(helper.c_str());
        }

    }
}


}
