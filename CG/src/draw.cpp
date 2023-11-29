#include "draw.hpp"
using namespace std;

bool drawpic(string path, vector<vector<int>> frameBuffer, int screenWidth, int screenHeight) {
    string OutputPath = path + ".ppm";
    ofstream outputFile(OutputPath);
    if (outputFile.is_open()) {
        // 写入PPM文件头
        outputFile << "P2" << endl;
        outputFile << screenWidth << " " << screenHeight << endl;
        outputFile << "255" << endl;

        // 写入图像数据
        for (int y = 0; y < screenHeight; ++y) {
            for (int x = 0; x < screenWidth; ++x) {
                outputFile << frameBuffer[screenWidth-x-1][screenHeight-y-1] << " ";  // 输出的图片是倒着的，将其正过来
            }
            outputFile << endl;
        }

        outputFile.close();
        return true;
    } 
    return false;
}