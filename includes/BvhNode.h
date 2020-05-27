//
// Created by fox-1942 on 4/23/20.
//

#ifndef RAYTRACERBOROS_BVHNODE_H
#define RAYTRACERBOROS_BVHNODE_H

#include "../Vendor/glm/glm.hpp"
#include <iostream>
#include <map>
#include <vector>
#include "../Vendor/glm/vec3.hpp"
#include "BBox.h"
#include <deque>

using namespace std;

int indDef = 0;
int numberOf = 1;
int numberOfLeaves = 0;

class BvhNode {

public:
    static const int &numberOfPolyInLeaf;

    int countNodes() const {
        for (int i = 0; i < this->children.size(); i++) {
            numberOf++;
            children.at(i)->countNodes();
        }
        return numberOf;
    }

public:
    BBox bBox;
    int depthOfNode;
    vector<BvhNode *> children;
    int order;
    bool isLeaf;
    bool createdEmpty = false;
    int leftOrRight;
    vector<glm::vec4> indices;

public:
    BvhNode() {}

    ~BvhNode() {}

    void buildTree(vector<glm::vec4> &indicesPerFaces, int depth) {

        if (indicesPerFaces.size() <= numberOfPolyInLeaf / 3) {
            this->bBox = BBox();
            this->indices = indicesPerFaces;
            this->depthOfNode = depth;
            this->isLeaf = true;
            this->order = indDef;
            this->createdEmpty = false;
            indDef++;
            return;
        }

        this->depthOfNode = depth;
        this->bBox = getBBox(indicesPerFaces);
        this->isLeaf = false;
        this->order = indDef;
        indDef++;

        int axis = this->bBox.longestAxis;

        vector<glm::vec4> leftTree;
        vector<glm::vec4> rightTree;

        for (int i = 0; i < indicesPerFaces.size(); ++i) {
            switch (axis) {
                case 0:
                    if (bBox.center.x >= this->bBox.faceCenters.at(i).x) {
                        leftTree.push_back(indicesPerFaces.at(i));
                    } else if (bBox.center.x < this->bBox.faceCenters.at(i).x) {
                        rightTree.push_back(indicesPerFaces.at(i));
                    }
                    break;
                case 1:
                    if (bBox.center.y >= this->bBox.faceCenters.at(i).y) {
                        leftTree.push_back(indicesPerFaces.at(i));
                    } else if (bBox.center.y < this->bBox.faceCenters.at(i).y) {
                        rightTree.push_back(indicesPerFaces.at(i));
                    }
                    break;
                case 2:
                    if (bBox.center.z >= this->bBox.faceCenters.at(i).z) {
                        leftTree.push_back(indicesPerFaces.at(i));
                    } else if (bBox.center.z < this->bBox.faceCenters.at(i).z) {
                        rightTree.push_back(indicesPerFaces.at(i));
                    }
            };
        }

        if (leftTree.size() == indicesPerFaces.size() || rightTree.size() == indicesPerFaces.size()) {
            this->indices = indicesPerFaces;
            this->isLeaf = true;
            return;
        }

        cout << "Left: " << leftTree.size() << endl;
        cout << "Right: " << rightTree.size() << endl;

        BvhNode *left = new BvhNode();
        BvhNode *right = new BvhNode();

        left->buildTree(leftTree, this->depthOfNode + 1);
        right->buildTree(rightTree, this->depthOfNode + 1);

        left->leftOrRight = 0;
        children.push_back(left);
        right->leftOrRight = 1;
        children.push_back(right);

        return;
    }


private:

    int getNumberOfLeaves() {
        for (int i = 0; i < this->children.size(); i++) {
            if (children.at(i)->isLeaf) {
                numberOfLeaves++;
            }
            children.at(i)->getNumberOfLeaves();
        }
        return numberOfLeaves;
    }

    int findDeep(int &deepest) {
        for (int i = 0; i < this->children.size(); i++) {
            if (this->children.at(i)->depthOfNode > deepest) {
                deepest = this->children.at(i)->depthOfNode;
            }
            children.at(i)->findDeep(deepest);
        }
        return deepest;
    }

    int getDeepestLevel() {
        int deepest = -1;
        return findDeep(deepest);
    }

    void treeComplete(int deepestLev) {
        if (this->children.empty() && this->depthOfNode != deepestLev) {
            this->isLeaf = true; // Commented, because semantically 'this' would remain a leaf;

            BvhNode *emptyNode = new BvhNode();
            emptyNode->bBox = BBox();
            emptyNode->createdEmpty = true;
            emptyNode->isLeaf = true;
            emptyNode->order = -1;
            emptyNode->depthOfNode = this->depthOfNode + 1;
            emptyNode->children.clear();
            emptyNode->leftOrRight = 0;
            this->children.push_back(emptyNode);

            BvhNode *emptyNode2 = new BvhNode();
            *emptyNode2 = *emptyNode;
            emptyNode2->leftOrRight = 1;
            this->children.push_back(emptyNode2);
        }

        /*if (this->children.size()==1 && this->depthOfNode != deepestLev) {
            BvhNode emptyNode;
            emptyNode.createdEmpty = true;
            emptyNode.depthOfNode = this->depthOfNode + 1;
            this->children.push_back(emptyNode);
        }*/

        for (int i = 0; i < this->children.size(); i++) {
            children.at(i)->treeComplete(deepestLev);
        }
    }

public:

    void makeBvHTreeComplete() {
        int deep = this->getDeepestLevel();
        this->treeComplete(deep);
    }

    int getNumberOfNodes() {
        numberOf = 1;
        return countNodes();
    }

    void InfoAboutNode() {
        numberOfLeaves = 0;

        cout << "Info about the tree:" << endl;
        cout << "------------------- " << endl;
        if (this->isLeaf) {
            cout << "Number Of leaves: " << 1 << endl;
        } else {
            cout << "Number Of leaves: " << getNumberOfLeaves() << endl;
        }
        cout << "Number Of nodes: " << getNumberOfNodes() << endl;
        cout << "Deepest level of the tree: " << getDeepestLevel() << "\n" << endl;
    }
};

#endif //RAYTRACERBOROS_BVHNODE_H