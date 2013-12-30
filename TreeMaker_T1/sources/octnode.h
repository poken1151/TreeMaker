/*
 * octnode.h
 *
 * Author:
 *     Owen Patrick
 *
 * School:
 *     Rochester Institute of Technology
 *
 * Version:
 *     2.0
 *
 * Description:
 *      Class descriptor for our octnode class. Contains a list of childeren
 *      to other octnodes as the scene is devided. Each leaf octnode also holds a list
 *      of branches that it contains.
 */
#ifndef OCTNODE_H
#define OCTNODE_H

#include "vecmath.h"
#include "branch.h"
#include <vector>

class Octnode {
    public:
        Octnode* child[8];
        int maxPoints;
        bool isLeaf;
        bool hasChild;
        float data;


        Octnode();
        Octnode(int height, int space, int maxP);
        Octnode(Point3 iAA, Point3 iBB, int maxP);
        ~Octnode();

        bool inMe(Point3& test);
        bool inMe(Branch& test);
        bool push(Branch* newBranch);
        void size(bool);
        int size() const;
        Point3 A() const;
        Point3 B() const;
        vector<Branch*> myPoints;

    private:
        int s;
        int maxDepth;
        Point3 AA;
        Point3 BB;
        Point3 centroid;
};

#endif
