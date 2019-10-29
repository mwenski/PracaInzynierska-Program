//
// Created by John on 19/10/29.
//

#ifndef PROJEKT_TRAJECTORY_H
#define PROJEKT_TRAJECTORY_H
#include<vector>
#include<string>
struct Point{
    float x[3],y[3],z[3];
};

struct Trajectory{
    std::vector<Point> coordinates;
    float step;
};
Trajectory trajectoryGenerator(std::string fileName);
#endif //PROJEKT_TRAJECTORY_H
