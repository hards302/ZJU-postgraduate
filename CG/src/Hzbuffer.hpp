#ifndef HZBUFFER_HPP
#define HZBUFFER_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
using std::vector;

class quadnode {
public:
    int minx, maxx, miny, maxy;        // Hzbuffer中，该层所代表的区域
    float z = FLT_MAX;                // Hzbuffer中，该层最远的像素点的z值

    vector<quadnode*> children;
    quadnode* father = nullptr;

    quadnode() {}
    quadnode(int minx, int maxx, int miny, int maxy) {
        this->minx = minx;
        this->maxx = maxx;
        this->miny = miny;
        this->maxy = maxy;
    }

    void release() {
        for(auto child: children) {
            if(child != nullptr) child->release();
        }
        children.clear();
    }
};

class HZbuffer {
private:
    int width, height;
    quadnode* midmap;
    quadnode* zbuffer;

public:
    vector<vector<int>> frameBuffer;

    HZbuffer() {};
    HZbuffer(int w, int h, vector<vector<int>> fB);
    // ~HZbuffer();

    void initialize();
    quadnode* getmidmap(int minx, int maxx, int miny, int maxy);

    void rendface(vector<Vertex> vertices, Face face, float facez, int minx, int maxx, int miny, int maxy, quadnode* node);
    void UpdateHZbuffer(vector<Vertex> vertices, Face face, quadnode* node);
    void render(vector<Vertex> vertices, vector<Face> faces);
};

#endif