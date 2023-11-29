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
    // 递归地删除所有节点
    // ...
    RecursiveDelete(root);
}

void RecursiveDelete(OctreeNode* node) {
    if (node == nullptr) {
        return;
    }

    // 递归删除每个子节点
    for (int i = 0; i < 8; i++) {
        RecursiveDelete(node->children[i]);
    }

    // 释放当前节点的内存
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

    // 计算当前节点的子节点边界框
    float mid_x = (node->bounds.min_x + node->bounds.max_x) / 2.0f;
    float mid_y = (node->bounds.min_y + node->bounds.max_y) / 2.0f;
    float mid_z = (node->bounds.min_z + node->bounds.max_z) / 2.0f;

    // 创建子节点
    for (int i = 0; i < 8; i++) {
        node->children[i] = new OctreeNode();
    }

    // 将三角形放入合适的子节点中
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

    // 检查当前节点的三角形与场景中的其他三角形进行深度测试和渲染
    // ...

    // 递归地渲染子节点
    // ...
}
