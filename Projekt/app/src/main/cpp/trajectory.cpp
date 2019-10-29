//
// Created by John on 19/10/29.
//

#include "trajectory.h"
#include <unistd.h>
#include<fstream>
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/vec3.hpp"
Trajectory trajectoryGenerator(std::string fileName)
{
    std::ifstream fr;
    fr.open(fileName);
    Trajectory traj;
    if(fr.failbit != std::ios_base::goodbit)
        return traj;
    std::string p;
    fr>>p; //first line is delta T for whole trajector;
    traj.step= atof(p.c_str());
    while(!fr.eof())
    {
        fr>>p;
        Point a;
        int comf=p.find(',');
        int coms = p.substr(comf).find(',');
        a.x[0]=atof(p.substr(0,comf).c_str());
        a.y[0]=atof(p.substr(comf,comf-coms).c_str());
        a.z[0]=atof(p.substr(coms).c_str());
        traj.coordinates.push_back(a);
    }

    for(int i = 0;i<traj.coordinates.size();i++) { //filling first and second delta
        if(i!=0) {
            traj.coordinates[i-1].x[1] = traj.coordinates[i].y[0] - traj.coordinates[i-1].y[0];
            traj.coordinates[i-1].y[1] = traj.coordinates[i].y[0] - traj.coordinates[i-1].y[0];
            traj.coordinates[i-1].z[1] = traj.coordinates[i].z[0] - traj.coordinates[i-1].z[0];
        }
        if(i!=1)
        {
            traj.coordinates[i-2].x[2] = traj.coordinates[i-1].y[1] - traj.coordinates[i-2].y[1];
            traj.coordinates[i-2].y[2] = traj.coordinates[i-1].y[1] - traj.coordinates[i-2].y[1];
            traj.coordinates[i-2].z[2] = traj.coordinates[i-1].z[1] - traj.coordinates[i-2].z[1];
        }
    }
    return traj;
}