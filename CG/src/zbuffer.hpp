#ifndef ZBUFFER_HPP
#define ZBUFFER_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
using std::vector;

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

float calculateZ(vector<Vertex> vertices, Face face, int x, int y);

vector<vector<int>> Z_Buffer(int screenWidth, int screenHeight, int maxz, int minx, int miny, vector<Vertex> vertices, vector<Face> faces);

#endif