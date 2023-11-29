#include "zbuffer.hpp"

using namespace std;

float calculateZ(vector<Vertex> vertices, Face face, int x, int y) {
    const Vertex& v1 = vertices[face.v1 - 1];
    const Vertex& v2 = vertices[face.v2 - 1];
    const Vertex& v3 = vertices[face.v3 - 1];
    float w1 = (-(x-v2.x)*(v3.y-v2.y)+(y-v2.y)*(v3.x-v2.x))/(-(v1.x-v2.x)*(v3.y-v2.y)+(v1.y-v2.y)*(v3.x-v2.x));
    float w2 = (-(x-v3.x)*(v1.y-v3.y)+(y-v3.y)*(v1.x-v3.x))/(-(v2.x-v3.x)*(v1.y-v3.y)+(v2.y-v3.y)*(v1.x-v3.x));
    float w3 = 1.0 - w1 - w2;
    if (w1 >= 0 && w2 >= 0 && w3 >= 0) return w1 * v1.z + w2 * v2.z + w3 * v3.z;
    else return FLT_MAX;
}

vector<vector<int>> Z_Buffer(int screenWidth, int screenHeight, int maxz, int minx, int miny, vector<Vertex> vertices, vector<Face> faces) {
    vector<vector<float>> zBuffer(screenWidth, vector<float>(screenHeight, FLT_MAX));
    vector<vector<int>> frameBuffer(screenWidth, vector<int>(screenHeight, 0));
    // 扫描每个要渲染的面
    for (auto face: faces) {
        const Vertex& v1 = vertices[face.v1 - 1];
        const Vertex& v2 = vertices[face.v2 - 1];
        const Vertex& v3 = vertices[face.v3 - 1];
        // 计算包围盒
        int min_x = min({v1.x, v2.x, v3.x});
        int max_x = max({v1.x, v2.x, v3.x});
        int min_y = min({v1.y, v2.y, v3.y});
        int max_y = max({v1.y, v2.y, v3.y});

        // 遍历包围盒中的像素点
        for (int x = min_x; x <= max_x; x++) {
            for (int y = min_y; y <= max_y; y++) {
                // 使用插值算法计算像素深度值
                float depth = calculateZ(vertices, face, x, y);
                if (depth < zBuffer[x][y]) { // obj文件中的坐标有负值，将所有坐标平移使得最小值为0
                    zBuffer[x][y] = depth;
                    frameBuffer[x][y] = 128; // 设置像素颜色
                }
            }
        }
    }
    return frameBuffer;
}