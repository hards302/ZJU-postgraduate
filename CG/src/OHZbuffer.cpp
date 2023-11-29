/*
八叉树层次zbuffer算法流程：
1. 构造midmap层次zbuffer，与Hzbuffer中构造的一样。
2. 构造场景八叉树。设置其深度depth为n，则对于每一个face，根据空间中均分为上下左右前后8个方块的位置插入到合适的子节点中（子节点的判断为深度等于n。此时八叉树的子节点保存着一组face，非子节点保存着其所有子节点的最小深度值z
3. 从根节点遍历八叉树的节点O。
    a. 找到节点O对应的midmap的准确节点Q（即正好包含节点O的范围），若该节点O保存的最小深度值大于midmap的节点Q（从根节点开始）保存的深度值，则该节点O的所有子节点都不绘制。
    b. 若O已经为根节点，则绘制其所有face，并更新midmap。
    c. 否则遍历O的所有子节点。
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
    // 分割到单个像素点，即底层zbuffer
    if(minx == maxx && miny == maxy) {
        int pos = miny * width + minx;
        return &(zbuffer[pos]);
    }
    quadnode* node = new quadnode(minx, maxx, miny, maxy);

    int midx = (minx + maxx)/2, midy = (miny + maxy)/2;
    // 将2*2大小的范围分割为node的子节点。限制条件是为了防止一个像素点重复分割。
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
    // 若到达最大深度或包围盒为单个像素，则将face放在八叉树的子节点中
    if(depth == OctreeMaxDepth || (node->minx == node->maxx && node->miny == node->maxy)) {
        node->faces.push_back(face);
        float z = min({vertices[face.v1-1].z,vertices[face.v2-1].z,vertices[face.v2-1].z});
        node->z = min(node->z, z);
        return;
    }
    // 否则，计算八叉树的八个子节点的边界
    int midx = (node->minx+node->maxx)/2;
    int midy = (node->miny+node->maxy)/2;
    float midz = (node->minz+node->maxz)/2;

    // 若node没有子节点，则创建8个子节点，边界为以上计算得到
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

    // 分别根据face的点的位置将face放到合适的子节点中。childnum为应该放置在第几号子节点的树中，一个face可能放置在多个八叉树叶节点中。
    int childnum1 = containdot(face.v1, midx, midy, midz);
    int childnum2 = containdot(face.v2, midx, midy, midz);
    int childnum3 = containdot(face.v3, midx, midy, midz);
    getscenerecusive(face, node->children[childnum1], depth+1);
    if(childnum2 != childnum1)
        getscenerecusive(face, node->children[childnum2], depth+1);
    if(childnum3 != childnum1 && childnum3 != childnum2)
        getscenerecusive(face, node->children[childnum3], depth+1);
    
    // 更新当前node的z值，为其所有子节点z值的最小值。即最靠近视野的深度值。
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
    // 找到包含onode范围的精确的qnode，若找不到，则跳过该onode。
    quadnode* Qnode = findexactqnode(onode, qnode);
    if(Qnode == nullptr) return;
    // 若onode的z值大于精确的qnode的z值，说明node中的所有face均被完全遮盖，跳过该哦node。
    if(onode->z > Qnode->z) return;
    // 若onode已经是叶节点，则绘制其所有的face，并更新qnode。
    if(onode->children.size() == 0) {
        for(auto face: onode->faces) {
            UpdateHZbuffer(face, midmap);
        }
    }
    // 递归绘制onode的所有子节点。
    for(auto child: onode->children) renderoct(child, Qnode);
}

quadnode* OHZbuffer::findexactqnode(octnode* onode, quadnode* qnode) {
    // 若onode完全不在qnode的范围中，返回null。
    if(onode->maxx <= qnode->minx || onode->minx >= qnode->maxx || onode->maxy <= qnode->miny || onode->miny >= qnode->maxy) return nullptr;
    // 若qnode是单像素，则返回qnode。
    if(qnode->children.size() == 0) return qnode;
    // 若qnode完全包围onode的范围
    if(onode->minx >= qnode->minx && onode->maxx <= qnode->maxx && onode->miny >= qnode->miny && onode->maxy <= qnode->maxy) {
        for(auto child: qnode->children) {
            quadnode* node = findexactqnode(onode, child);
            if(node != nullptr) return node;
        }
    }
    // 若qnode既不被完全排除，又没有完全包围，说明是部分包围，而我们是从上到下检查的，所以此时精确的包围盒一定是其父节点。
    return qnode->father;
}

void OHZbuffer::UpdateHZbuffer(Face face, quadnode* node) {
    if(node->children.size() == 0) {
        float depth = calculateZ(vertices, face, node->minx, node->miny);
        if (depth < node->z) {
            // cout << "画点 " << node->minx << " " << node->miny << endl; 
            node->z = depth;
            frameBuffer[node->minx][node->miny] = 128;
        }
        return;
    }
    // 否则，更新孩子的层次zbuffer值，选择其最大值为当前层zbuffer的值。
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