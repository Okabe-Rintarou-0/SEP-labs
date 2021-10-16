#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

#include "Tree.h"

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

int TreeNode::getX() {
    return data[0];
}

int TreeNode::getY() {
    return data[1];
}

int TreeNode::getValue(int dim) const {
    return data[dim];
}

TreeNode::TreeNode(int x, int y, TreeNode *leftChild, TreeNode *rightChild) : leftChild(leftChild),
                                                                              rightChild(rightChild) {
    data[0] = x;
    data[1] = y;
}

TreeNode::~TreeNode() {
}

ostream &operator<<(ostream &out, const TreeNode &b) {
    return out << "(" << b.getValue(0) << ", " << b.getValue(1) << ")";
}

void BinaryDimonTree::sortByDim(vector<TreeNode *> &nodes, int l, int r, int dim) {
    sort(nodes.begin() + l, nodes.begin() + r + 1, [=](const TreeNode *a, const TreeNode *b) {
        return a->getValue(dim) < b->getValue(dim);
    });
}

void BinaryDimonTree::create(vector<TreeNode *> &nodes, int l, int r, TreeNode **parent, int dim) {
    if (l >= r) {
        if (l == r)
            *parent = nodes[l];
        return;
    }

    sortByDim(nodes, l, r, dim);
    int mid = (l + r) / 2;
    *parent = nodes[mid];

    // recursively create
    dim = (dim + 1) % 2;
    create(nodes, l, mid - 1, &(*parent)->leftChild, dim);
    create(nodes, mid + 1, r, &(*parent)->rightChild, dim);
}

void BinaryDimonTree::createTree(vector<TreeNode *> &nodes) {
    create(nodes, 0, nodes.size() - 1, &root, 0);
}

istream &operator>>(istream &in, BinaryDimonTree &tree) {
    int M;
    in >> M;
    vector < TreeNode * > nodes;
    int x, y;
    for (int i = 0; i < M; ++i) {
        in >> x >> y;
        nodes.push_back(new TreeNode(x, y));
    }
    tree.createTree(nodes);
    return in;
}

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void BinaryDimonTree::recur_search(TreeNode *cur, int x, int y, double &min_distance, TreeNode **guess, int dim) {
    if (cur == nullptr)
        return;

    double curDis = distance(cur->getX(), cur->getY(), x, y);

//    cout << "Trying to find nearest point of (" << x << ", " << y << "), current min dis: " << min_distance << endl;
//    cout << "Current dis: " << curDis << endl;

    if (curDis < min_distance) {
        min_distance = curDis;
        *guess = cur;
//        cout << "Update guess to " << *cur << endl;
    }

    int curV = cur->getValue(dim);
    int tgtV = dim ? y : x;
    int nextDim = (dim + 1) % 2;
    TreeNode *next = tgtV < curV ? cur->leftChild : cur->rightChild;
    TreeNode *another = tgtV < curV ? cur->rightChild : cur->leftChild;

    /*
     * Search on the side where the target point belongs to.
     */
    recur_search(next, x, y, min_distance, guess, nextDim);

    /*
     * Image the circle. If the vertical or the horizontal distance
     * is smaller than current minimum distance, that means it is possible
     * that there are some nearer points on the another side of the axis.
     */
    if (abs(tgtV - curV) < min_distance)
        recur_search(another, x, y, min_distance, guess, nextDim);
}

TreeNode *BinaryDimonTree::find_nearest_node(int x, int y) {
    TreeNode *nearest = nullptr;
    double min_distance = numeric_limits<double>::max();
    recur_search(root, x, y, min_distance, &nearest, 0);
    return nearest;
}

BinaryDimonTree::BinaryDimonTree() : root(nullptr) {}