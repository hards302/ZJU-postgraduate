/*
�˲������zbuffer�㷨���̣�
1. ����midmap���zbuffer����Hzbuffer�й����һ����
2. ���쳡���˲��������������depthΪn�������ÿһ��face�����ݿռ��о���Ϊ��������ǰ��8�������λ�ò��뵽���ʵ��ӽڵ��У��ӽڵ���ж�Ϊ��ȵ���n����ʱ�˲������ӽڵ㱣����һ��face�����ӽڵ㱣�����������ӽڵ����С���ֵz
3. �Ӹ��ڵ�����˲����Ľڵ�O��
    a. �ҵ��ڵ�O��Ӧ��midmap��׼ȷ�ڵ�Q�������ð����ڵ�O�ķ�Χ�������ýڵ�O�������С���ֵ����midmap�Ľڵ�Q���Ӹ��ڵ㿪ʼ����������ֵ����ýڵ�O�������ӽڵ㶼�����ơ�
    b. ��O�Ѿ�Ϊ���ڵ㣬�����������face��������midmap��
    c. �������O�������ӽڵ㡣
*/
#include "OHzbuffer.hpp"
#include <iostream>

using namespace std;

OHZbuffer::OHZbuffer(int w, int h, vector<Vertex> v, vector<Face> f, vector<vector<int>> fB) {
    width = w;
    height = h;
    vertices = v;
    faces = f;
    frameBuffer = fB;
}

void OHZbuffer::initialize(int minx, int maxx, int miny, int maxy, float minz, float maxz) {
    zbuffer = new quadnode[width * height];
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            int pos = j * width + i;
            zbuffer[pos].minx = zbuffer[pos].maxx = i;
            zbuffer[pos].miny = zbuffer[pos].maxy = j;
            zbuffer[pos].z = FLT_MAX;
        }
    }
    OctreeMaxDepth = log(faces.size()) / log(8);
    midmap = getmidmap(0, width-1, 0, height-1);
    scene = getscene(minx, maxx, miny, maxy, minz, maxz);
}

quadnode* OHZbuffer::getmidmap(int minx, int maxx, int miny, int maxy) {
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

octnode* OHZbuffer::getscene(int minx, int maxx, int miny, int maxy, float minz, float maxz) {
    octnode* Onode = new octnode(minx, maxx, miny, maxy, minz, maxz);
    for(auto face: faces) {
        getscenerecusive(face, Onode, 0);
    }
    return Onode;
}

void OHZbuffer::getscenerecusive(Face face, octnode* node, int depth) {
    if(node->minx > node->maxx || node->miny > node->maxy || node->minz > node->maxz) return;
    // �����������Ȼ��Χ��Ϊ�������أ���face���ڰ˲������ӽڵ���
    if(depth == OctreeMaxDepth || (node->minx == node->maxx && node->miny == node->maxy)) {
        node->faces.push_back(face);
        float z = min({vertices[face.v1-1].z,vertices[face.v2-1].z,vertices[face.v2-1].z});
        node->z = min(node->z, z);
        return;
    }
    // ���򣬼���˲����İ˸��ӽڵ�ı߽�
    int midx = (node->minx+node->maxx)/2;
    int midy = (node->miny+node->maxy)/2;
    float midz = (node->minz+node->maxz)/2;

    // ��nodeû���ӽڵ㣬�򴴽�8���ӽڵ㣬�߽�Ϊ���ϼ���õ�
    if(node->children.size() == 0) {
        node->children.push_back(new octnode(node->minx, midx, node->miny, midy, node->minz, midz));
        node->children.push_back(new octnode(node->minx, midx, node->miny, midy, midz+1, node->maxz));
        node->children.push_back(new octnode(node->minx, midx, midy+1, node->maxy, node->minz, midz));
        node->children.push_back(new octnode(node->minx, midx, midy+1, node->maxy, midz+1, node->maxz));
        node->children.push_back(new octnode(midx+1, node->maxx, node->miny, midy, node->minz, midz));
        node->children.push_back(new octnode(midx+1, node->maxx, node->miny, midy, midz+1, node->maxz));
        node->children.push_back(new octnode(midx+1, node->maxx, midy+1, node->maxy, node->minz, midz));
        node->children.push_back(new octnode(midx+1, node->maxx, midy+1, node->maxy, midz+1, node->maxz));
    }

    // �ֱ����face�ĵ��λ�ý�face�ŵ����ʵ��ӽڵ��С�childnumΪӦ�÷����ڵڼ����ӽڵ�����У�һ��face���ܷ����ڶ���˲���Ҷ�ڵ��С�
    int childnum1 = containdot(face.v1, midx, midy, midz);
    int childnum2 = containdot(face.v2, midx, midy, midz);
    int childnum3 = containdot(face.v3, midx, midy, midz);
    getscenerecusive(face, node->children[childnum1], depth+1);
    if(childnum2 != childnum1)
        getscenerecusive(face, node->children[childnum2], depth+1);
    if(childnum3 != childnum1 && childnum3 != childnum2)
        getscenerecusive(face, node->children[childnum3], depth+1);
    
    // ���µ�ǰnode��zֵ��Ϊ�������ӽڵ�zֵ����Сֵ���������Ұ�����ֵ��
    for(auto child: node->children) node->z = min(node->z, child->z);
}

int OHZbuffer::containdot(int facenum, int midx, int midy, float midz) {
    int x = vertices[facenum-1].x;
    int y = vertices[facenum-1].y;
    float z = vertices[facenum-1].z;
    if(x <= midx) {
        if(y <= midy) {
            if(z <= midz) return 0;
            else return 1;
        }
        else {
            if(z <= midz) return 2;
            else return 3;
        }
    }
    else {
        if(y <= midy) {
            if(z <= midz) return 4;
            else return 5;
        }
        else {
            if(z <= midz) return 6;
            else return 7;
        }
    }
}

void OHZbuffer::renderoct(octnode* onode, quadnode* qnode) {
    // �ҵ�����onode��Χ�ľ�ȷ��qnode�����Ҳ�������������onode��
    quadnode* Qnode = findexactqnode(onode, qnode);
    if(Qnode == nullptr) return;
    // ��onode��zֵ���ھ�ȷ��qnode��zֵ��˵��node�е�����face������ȫ�ڸǣ�������Ŷnode��
    if(onode->z > Qnode->z) return;
    // ��onode�Ѿ���Ҷ�ڵ㣬����������е�face��������qnode��
    if(onode->children.size() == 0) {
        for(auto face: onode->faces) {
            UpdateHZbuffer(face, midmap);
        }
    }
    // �ݹ����onode�������ӽڵ㡣
    for(auto child: onode->children) renderoct(child, Qnode);
}

quadnode* OHZbuffer::findexactqnode(octnode* onode, quadnode* qnode) {
    // ��onode��ȫ����qnode�ķ�Χ�У�����null��
    if(onode->maxx <= qnode->minx || onode->minx >= qnode->maxx || onode->maxy <= qnode->miny || onode->miny >= qnode->maxy) return nullptr;
    // ��qnode�ǵ����أ��򷵻�qnode��
    if(qnode->children.size() == 0) return qnode;
    // ��qnode��ȫ��Χonode�ķ�Χ
    if(onode->minx >= qnode->minx && onode->maxx <= qnode->maxx && onode->miny >= qnode->miny && onode->maxy <= qnode->maxy) {
        for(auto child: qnode->children) {
            quadnode* node = findexactqnode(onode, child);
            if(node != nullptr) return node;
        }
    }
    // ��qnode�Ȳ�����ȫ�ų�����û����ȫ��Χ��˵���ǲ��ְ�Χ���������Ǵ��ϵ��¼��ģ����Դ�ʱ��ȷ�İ�Χ��һ�����丸�ڵ㡣
    return qnode->father;
}

void OHZbuffer::UpdateHZbuffer(Face face, quadnode* node) {
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
            UpdateHZbuffer(face, child);
            newz = max(newz, child->z);
        }
        node->z = newz;
        return;
    }
}