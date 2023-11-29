#include <iostream>
#include <chrono>
#include "zbuffer.hpp"
#include "Hzbuffer.hpp"
#include "OHzbuffer.hpp"
#include "draw.hpp"

using namespace std;

string input_path = "../model/";
string output_path = "./output/";
string model_name = "cat";

int main() {
    // ��ȡ.obj�ļ�
    string ModelName = input_path+model_name+".obj";
    ifstream file(ModelName);
    string line;
    vector<Vertex> vertices;
    vector<Face> faces;
    float maxz = 0, minz = FLT_MAX, minx = 0, maxx = 0, miny = 0, maxy = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream iss(line);
            string prefix;
            iss >> prefix;
            if (prefix == "v") {
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
                minz = min(minz, vertex.z);
                maxz = max(maxz, vertex.z);
                minx = min(minx, vertex.x);
                maxx = max(maxx, vertex.x);
                miny = min(miny, vertex.y);
                maxy = max(maxy, vertex.y);
            }
            else if (prefix == "f") {
                Face face;
                string s1, s2, s3;
                iss >> face.v1 >> s1 >> face.v2 >> s2 >> face.v3 >> s3;
                faces.push_back(face);
            }
        }

        file.close();
    }
    else {
        cout << "no such file named " << ModelName << endl;
        return 0;
    }
    cout << "�ļ���ȡ�ɹ���" << endl;
    // cout << vertices.size() << " " << faces.size() << endl;
    // ������Ļ�ߴ��Z������
    const int screenWidth = maxx-minx+1;
    const int screenHeight = maxy-miny+1;
    // vector<vector<float>> zBuffer(screenWidth, vector<float>(screenHeight, maxz));
    // // cout << "maxz = " << maxz << " zbuffermax = " << zBuffer[0][0] << endl;
    for(int i = 0; i < vertices.size(); i++) {
        vertices.at(i).x -= minx;
        vertices.at(i).y -= miny;
    }
    vector<vector<int>> frameBuffer(screenWidth, vector<int>(screenHeight, 0));
    // HZbuffer hzbuffer(screenWidth, screenHeight, frameBuffer);
    OHZbuffer ohzbuffer(screenWidth, screenHeight, vertices, faces, frameBuffer);
    cout << "��ʼ����ʼ" << endl;
    ohzbuffer.initialize(0, maxx-minx, 0, maxy-miny, minz, maxz);
    cout << "��ʼ������" << endl;
    auto start = chrono::high_resolution_clock::now();
    // frameBuffer = Z_Buffer(screenWidth, screenHeight, maxz, minx, miny, vertices, faces);
    ohzbuffer.renderoct(ohzbuffer.scene, ohzbuffer.midmap);
    auto end = chrono::high_resolution_clock::now();
    // �����㷨����ʱ��
    chrono::duration<double> duration = end - start;
    double timeTaken = duration.count();
    cout << "�㷨����ʱ�䣺" << timeTaken << "s" << endl;
    // ����ppm�Ҷ�ͼ
    cout << "����ͼƬ��..." << endl;
    if(drawpic(output_path+model_name+"OHZ", ohzbuffer.frameBuffer, screenWidth, screenHeight)) cout << "ͼƬ�����ɹ�!" << endl;
    else cout << "�޷�����ͼƬ" << endl;

    return 0;
}