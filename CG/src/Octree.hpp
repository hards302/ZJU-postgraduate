#ifndef OCTREE_HPP
#define OCTREE_HPP
#include "zbuffer.hpp"
#include <vector>
using std::vector;

class octnode {
public:
    octnode() {}
    octnode(int minx, int maxx, int miny, int maxy, float minz, float maxz) {
        this->minx = minx;
        this->maxx = maxx; 
        this->miny = miny;
        this->maxy = maxy;
        this->minz = minz;
        this->maxz = maxz;
    }

    float z = FLT_MAX;    // 保存该节点区域物体的最近深度值
    int minx, maxx, miny, maxy;
    float minz, maxz;
    vector<Vertex> vertices;
    vector<Face> faces;
    vector<octnode*> children;
};

#endif
