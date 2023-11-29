#ifndef OHZBUFFER_HPP
#define OHZBUFFER_HPP
#include "Octree.hpp"
#include "Hzbuffer.hpp"

class OHZbuffer {
public:
    int width, height;
    quadnode* midmap;
    quadnode* zbuffer;
    vector<Vertex> vertices;
    vector<Face> faces;
    octnode* scene;
    vector<vector<int>> frameBuffer;
    int OctreeMaxDepth;

    OHZbuffer() {};
    OHZbuffer(int w, int h, vector<Vertex> v, vector<Face> f, vector<vector<int>> fB);

    void initialize(int minx, int maxx, int miny, int maxy, float minz, float maxz);
    quadnode* getmidmap(int minx, int maxx, int miny, int maxy);
    octnode* getscene(int minx, int maxx, int miny, int maxy, float minz, float maxz);
    void getscenerecusive(Face face, octnode* node, int depth);
    int containdot(int facenum, int midx, int midy, float midz);

    void renderoct(octnode* onode, quadnode* qnode);
    quadnode* findexactqnode(octnode* onode, quadnode* qnode);
    // void rendface(vector<Vertex> vertices, Face face, float facez, int minx, int maxx, int miny, int maxy, quadnode* node);
    void UpdateHZbuffer(Face face, quadnode* node);
    // void render(vector<Vertex> vertices, vector<Face> faces);
};

#endif