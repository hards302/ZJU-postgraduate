#include "Octree.hpp"
using namespace std;


Octree::Octree(const BoundingBox& bounds, int depth) {
    root = new OctreeNode();
    root->bounds = bounds;
    maxDepth = depth;
    for (int i = 0; i < 8; i++) {
        root->children[i] = nullptr;
    }
}

Octree::~Octree() {
    // �ݹ��ɾ�����нڵ�
    // ...
    RecursiveDelete(root);
}

void RecursiveDelete(OctreeNode* node) {
    if (node == nullptr) {
        return;
    }

    // �ݹ�ɾ��ÿ���ӽڵ�
    for (int i = 0; i < 8; i++) {
        RecursiveDelete(node->children[i]);
    }

    // �ͷŵ�ǰ�ڵ���ڴ�
    delete node;
}

void Octree::InsertTriangle(const Face& triangle, const vector<Vertex>& vertices) {
    InsertTriangleRecursive(root, triangle, vertices, 0);
}

void Octree::InsertTriangleRecursive(OctreeNode* node, const Face& triangle, const //, int depth) {
    if (depth == maxDepth) {
        node->triangles.push_back(triangle);
        return;
    }

    // ���㵱ǰ�ڵ���ӽڵ�߽��
    float mid_x = (node->bounds.min_x + node->bounds.max_x) / 2.0f;
    float mid_y = (node->bounds.min_y + node->bounds.max_y) / 2.0f;
    float mid_z = (node->bounds.min_z + node->bounds.max_z) / 2.0f;

    // �����ӽڵ�
    for (int i = 0; i < 8; i++) {
        node->children[i] = new OctreeNode();
    }

    // �������η�����ʵ��ӽڵ���
    if (vertices[triangle.v1-1].x <= mid_x) {
        if (vertices[triangle.v1-1].y <= mid_y) {
            if (vertices[triangle.v1-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[0], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[1], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v1-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[2], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[3], triangle, vertices, depth + 1);
            }
        }
    } else {
        if (vertices[triangle.v1-1].y <= mid_y) {
            if (vertices[triangle.v1-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[4], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[5], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v1-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[6], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[7], triangle, vertices, depth + 1);
            }
        }
    }
    if (vertices[triangle.v2-1].x <= mid_x) {
        if (vertices[triangle.v2-1].y <= mid_y) {
            if (vertices[triangle.v2-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[0], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[1], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v2-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[2], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[3], triangle, vertices, depth + 1);
            }
        }
    } else {
        if (vertices[triangle.v2-1].y <= mid_y) {
            if (vertices[triangle.v2-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[4], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[5], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v2-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[6], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[7], triangle, vertices, depth + 1);
            }
        }
    }
    if (vertices[triangle.v3-1].x <= mid_x) {
        if (vertices[triangle.v3-1].y <= mid_y) {
            if (vertices[triangle.v3-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[0], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[1], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v3-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[2], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[3], triangle, vertices, depth + 1);
            }
        }
    } else {
        if (vertices[triangle.v3-1].y <= mid_y) {
            if (vertices[triangle.v3-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[4], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[5], triangle, vertices, depth + 1);
            }
        } else {
            if (vertices[triangle.v3-1].z <= mid_z) {
                InsertTriangleRecursive(node->children[6], triangle, vertices, depth + 1);
            } else {
                InsertTriangleRecursive(node->children[7], triangle, vertices, depth + 1);
            }
        }
    }
}

void Octree::Render() {
    RenderRecursive(root);
}

void Octree::RenderRecursive(OctreeNode* node) {
    if (node == nullptr) {
        return;
    }

    // ��鵱ǰ�ڵ���������볡���е����������ν�����Ȳ��Ժ���Ⱦ
    // ...

    // �ݹ����Ⱦ�ӽڵ�
    // ...
}
