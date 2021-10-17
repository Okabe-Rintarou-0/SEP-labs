//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

class TreeNode {
    friend ostream &operator<<(ostream &out, const TreeNode &b);

    friend class BinaryTree;

    friend class BinaryDimonTree;

private:
    /* data */
    int data[2];

    TreeNode *leftChild;
    TreeNode *rightChild;

public:
    /* methods */
    int getX();  /* DO NOT CHANGE */
    int getY();  /* DO NOT CHANGE */
    int getValue(int dim) const;

    TreeNode(int x, int y, TreeNode *leftChild = nullptr, TreeNode *rightChild = nullptr);

    ~TreeNode(); /* DO NOT CHANGE */
};


class BinaryDimonTree {
    friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
    /* data */
    TreeNode *root;

    void sortByDim(vector<TreeNode *> &nodes, int l, int r, int dim);

    void create(vector<TreeNode *> &nodes, int l, int r, TreeNode **parent, int dim);

public:
    /* methods */
    BinaryDimonTree();          /* DO NOT CHANGE */
    ~BinaryDimonTree();
    TreeNode *find_nearest_node(int x, int y);  /* DO NOT CHANGE */

    void createTree(vector<TreeNode *> &nodes);

    void recur_search(TreeNode *cur, int x, int y, double &min_distance, TreeNode **guess, int dim);

};

#endif //C_BINARYDIMEN_TREE_H
