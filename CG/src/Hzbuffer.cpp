/*
���zbuffer�㷨���̣�
1. �����ʼzbuffer�����ײ�zbuffer�����zbuffer�㷨�е���һ���ġ�
2. ���ݳ�ʼzbuffer��ʹ���Ĳ����ṹ����midmap���zbuffer��
3. ����ÿ��face�����ж������ô�����һ��midmap��
    �ж�����һ���У�face�Ƿ���ȫ���ɼ�
        �����ɼ�����������face��
        ���ɼ�����update���zbuffer��������ʾ�����ĵ㻭����
*/
#include "zbuffer.hpp"
#include "Hzbuffer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

HZbuffer::HZbuffer(int w, int h, vector<vector<int>> fB) {
    width = w;
    height = h;
    frameBuffer = fB;
    initialize();
}

void HZbuffer::initialize() {
    zbuffer = new quadnode[width * height];
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            int pos = j * width + i;
            zbuffer[pos].minx = zbuffer[pos].maxx = i;
            zbuffer[pos].miny = zbuffer[pos].maxy = j;
            zbuffer[pos].z = FLT_MAX;
        }
    }
    midmap = getmidmap(0, width-1, 0, height-1);
}

quadnode* HZbuffer::getmidmap(int minx, int maxx, int miny, int maxy) {
    // �ָ�������ص㣬���ײ�zbuffer
    if(minx == maxx && miny == maxy) {
        int pos = miny * width + minx;
        return &(zbuffer[pos]);
    }
    quadnode* node = new quadnode(minx, maxx, miny, maxy);

    int midx = (minx + maxx)/2, midy = (miny + maxy)/2;
    // ��2*2��С�ķ�Χ�ָ�Ϊnode���ӽڵ㡣����������Ϊ�˷�ֹһ�����ص��ظ��ָ
    if(midx >= minx && midy >= miny) {
        quadnode* child = getmidmap(minx, midx, miny, midy);
        node->children.push_back(child);
        child->father = node;
    }
    if(midx < maxx) {
        quadnode* child = getmidmap(midx+1, maxx, miny, midy);
        node->children.push_back(child);
        child->father = node;
    }
    if(midy < maxy) {
        quadnode* child = getmidmap(minx, midx, midy+1, maxy);
        node->children.push_back(child);
        child->father = node;
    }
    if(midx < maxx && midy < maxy) {
        quadnode* child = getmidmap(midx+1, maxx, midy+1, maxy);
        node->children.push_back(child);
        child->father = node;
    }
    return node;
}
void HZbuffer::rendface(vector<Vertex> vertices, Face face, float facez, int minx, int maxx, int miny, int maxy, quadnode* node) {
    if(facez >= node->z) return;   // ��face�����ֵ���ڴ�ʱzbuffer���zֵ��˵������ȫ�ڵ���
    else {
        bool exact = true;            // ��ʾ�Ƿ�Ϊface���ڵ���һ�㡣
        int childnum = 0;
        for(childnum = 0; childnum < node->children.size(); childnum++) {
            if(minx >= node->children[childnum]->minx && maxx <= node->children[childnum]->maxx && miny >= node->children[childnum]->miny && maxy <= node->children[childnum]->maxy) {
                exact = false;
                break;
            }
        }
        if (!exact) rendface(vertices, face, facez, minx, maxx, miny, maxy, node->children[childnum]);
        else UpdateHZbuffer(vertices, face, node);
    }
}

void HZbuffer::UpdateHZbuffer(vector<Vertex> vertices, Face face, quadnode* node) {
    if(node->children.size() == 0) {
        float depth = calculateZ(vertices, face, node->minx, node->miny);
        if (depth < node->z) {
            // cout << "���� " << node->minx << " " << node->miny << endl; 
            node->z = depth;
            frameBuffer[node->minx][node->miny] = 128;
        }
        return;
    }
    // ���򣬸��º��ӵĲ��zbufferֵ��ѡ�������ֵΪ��ǰ��zbuffer��ֵ��
    else {
        float newz = -FLT_MAX;
        for(auto child: node->children) {
            UpdateHZbuffer(vertices, face, child);
            newz = max(newz, child->z);
        }
        node->z = newz;
        return;
    }
}

void HZbuffer::render(vector<Vertex> vertices, vector<Face> faces) {
    int facecount = 0;
    for(auto face: faces) {
        const Vertex& v1 = vertices[face.v1 - 1];
        const Vertex& v2 = vertices[face.v2 - 1];
        const Vertex& v3 = vertices[face.v3 - 1];
        // �����Χ�к���С��zֵ
        int min_x = min({v1.x, v2.x, v3.x});
        int max_x = max({v1.x, v2.x, v3.x});
        int min_y = min({v1.y, v2.y, v3.y});
        int max_y = max({v1.y, v2.y, v3.y});
        int min_z = min({v1.z, v2.z, v3.z});
        // ��Ⱦ����
        facecount++;
        cout << "���ƽ��ȣ�" << facecount << "/" << faces.size() << endl;
        rendface(vertices, face, min_z, min_x, max_x, min_y, max_y, midmap);
    }
}


// quadnode* getmidmap(int minx, int maxx, int miny, int maxy, int w, quadnode* zBuffer) {
//     // �ָ�������ص㣬���ײ�zbuffer
//     if(minx == maxx && miny == maxy) {
//         int loc = miny * w + minx;
//         return &(zBuffer[loc]);
//     }
    
//     quadnode* node = new quadnode(minx, maxx, miny, maxy);

//     int midx = (minx + maxx) / 2;
//     int midy = (miny + maxy) / 2;
    
//     // ��2*2��С�ķ�Χ�ָ�Ϊnode���ӽڵ㡣����������Ϊ�˷�ֹһ�����ص��ظ��ָ
//     if(midx >= minx && midy >= miny) {
//         quadnode* child = getmidmap(minx, midx, miny, midy, w, zBuffer);
//         node->children.push_back(child);
//         child->father = node;
//     }
//     if(midx < maxx) {
//         quadnode* child = getmidmap(midx+1, maxx, miny, midy, w, zBuffer);
//         node->children.push_back(child);
//         child->father = node;
//     }
//     if(midy < maxy) {
//         quadnode* child = getmidmap(minx, midx, midy+1, maxy, w, zBuffer);
//         node->children.push_back(child);
//         child->father = node;
//     }
//     if(midx < maxx && midy < maxy) {
//         quadnode* child = getmidmap(midx+1, maxx, midy+1, maxy, w, zBuffer);
//         node->children.push_back(child);
//         child->father = node;
//     }

//     return node;
// }
// vector<vector<int>> rendface(vector<vector<int>> frameBuffer, vector<Vertex> vertices, Face face, float facez, int minx, int maxx, int miny, int maxy, quadnode* node) {
//     // if(countnumber % 100 == 0) 
//     cout << "������Ⱦ��" << countnumber << endl;
//     if(facez >= node->z) return frameBuffer;   // ��face�����ֵ���ڴ�ʱzbuffer���zֵ��˵������ȫ�ڵ���
//     bool exact = true;            // ��ʾ�Ƿ�Ϊface���ڵ���һ�㡣
//     int childnum = 0;
//     for(childnum = 0; childnum < node->children.size(); childnum++) {
//         if(minx >= node->children[childnum]->minx && maxx <= node->children[childnum]->maxx && miny >= node->children[childnum]->miny && maxy <= node->children[childnum]->maxy) {
//         exact = false;
//         break;
//         }
//     }
//     cout << "exact = " << exact << endl;
//     if (!exact) frameBuffer = rendface(frameBuffer, vertices, face, facez, minx, maxx, miny, maxy, node->children[childnum]);
//     else frameBuffer = UpdateHZbuffer(frameBuffer, vertices, face, node);
//     cout << "���ڸ�����" << countnumber << endl;
//     countnumber += 1;
//     return frameBuffer;
// }

// vector<vector<int>> UpdateHZbuffer(vector<vector<int>> frameBuffer, vector<Vertex> vertices, Face face, quadnode* node) {
//     // if(node->minx > node->maxx || node->miny > node->maxy) return frameBuffer;
//     // ���ǵײ�zbuffer����������ֵѡ����Сֵ�����£�������������Ⱦ��
//     if(node->children.size() == 0) {
//         float depth = calculateZ(vertices, face, node->minx, node->miny);
//         if (depth < node->z) {
//             // cout << "���� " << node->minx << " " << node->miny << endl; 
//             node->z = depth;
//             frameBuffer[node->minx][node->miny] = 128;
//         }
//         return frameBuffer;
//     }
//     // ���򣬸��º��ӵĲ��zbufferֵ��ѡ�������ֵΪ��ǰ��zbuffer��ֵ��
//     else {
//         float newz = -FLT_MAX;
//         for(auto child: node->children) {
//             frameBuffer = UpdateHZbuffer(frameBuffer, vertices, face, child);
//             newz = max(newz, child->z);
//         }
//         node->z = newz;
//         return frameBuffer;
//     }
// }

// vector<vector<int>> HZ_Buffer(int screenWidth, int screenHeight, int maxz, int minx, int miny, vector<Vertex> vertices, vector<Face> faces) {
//     quadnode* zBuffer = new quadnode[screenWidth * screenHeight];
//     vector<vector<int>> frameBuffer(screenWidth, vector<int>(screenHeight, 0));
//     for(int j = 0; j < screenHeight; j++) {
//         for(int i = 0; i < screenWidth; i++) {
//             int loc = j*screenWidth + i;
//             zBuffer[loc].minx = i;
//             zBuffer[loc].maxx = i;
//             zBuffer[loc].miny = j;
//             zBuffer[loc].maxy = j;
//         }
//     }
//     cout << "���zbuffer������..." << endl;
//     quadnode* root = getmidmap(0, screenWidth-1, 0, screenHeight-1, screenWidth, zBuffer);
//     cout << "���zbuffer�����ɹ���" << endl;
//     int countface = 0;
//     for(auto face: faces) {
//         const Vertex& v1 = vertices[face.v1 - 1];
//         const Vertex& v2 = vertices[face.v2 - 1];
//         const Vertex& v3 = vertices[face.v3 - 1];
//         // �����Χ�к���С��zֵ
//         int min_x = min({v1.x, v2.x, v3.x});
//         int max_x = max({v1.x, v2.x, v3.x});
//         int min_y = min({v1.y, v2.y, v3.y});
//         int max_y = max({v1.y, v2.y, v3.y});
//         int min_z = min({v1.z, v2.z, v3.z});
//         // ��Ⱦ����
//         countface += 1;
//         cout << "���ȣ�" << countface << "/" << faces.size() << endl;
//         frameBuffer = rendface(frameBuffer, vertices, face, min_z, min_x, max_x, min_y, max_y, root);
//         if(countface == 1000) break;
//     }
//     return frameBuffer;
// }