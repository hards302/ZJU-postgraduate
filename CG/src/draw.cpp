#include "draw.hpp"
using namespace std;

bool drawpic(string path, vector<vector<int>> frameBuffer, int screenWidth, int screenHeight) {
    string OutputPath = path + ".ppm";
    ofstream outputFile(OutputPath);
    if (outputFile.is_open()) {
        // д��PPM�ļ�ͷ
        outputFile << "P2" << endl;
        outputFile << screenWidth << " " << screenHeight << endl;
        outputFile << "255" << endl;

        // д��ͼ������
        for (int y = 0; y < screenHeight; ++y) {
            for (int x = 0; x < screenWidth; ++x) {
                outputFile << frameBuffer[screenWidth-x-1][screenHeight-y-1] << " ";  // �����ͼƬ�ǵ��ŵģ�����������
            }
            outputFile << endl;
        }

        outputFile.close();
        return true;
    } 
    return false;
}