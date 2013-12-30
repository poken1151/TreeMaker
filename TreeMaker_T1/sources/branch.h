/*
 * branch.h
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
 *     Class descriptor for branches. Holds a list of children and parent
 *     pointer. Also holds a start and end node pointer and a growth
 *     direction aggregator to spawn a new branch.
 */
#ifndef BRANCH_H
#define BRANCH_H

#include "vecmath.h"
#include <list>

class Branch {
    public:
        Branch();
        Branch(Point3* start, Point3* end, int date);
        Branch(const Point3 start, const Point3 end, int date, int lu);
        Branch(const Point3 start, const Vector3 way, int date, int lu);
        ~Branch();
        Vector3 rot;
        double fangle;
        int spawn;
        int lookUp;

        Point3* startNodeTest;
        Point3* endNodeTest;

        const Point3& node() const;
        const Point3& middle() const;
        const Vector3& direction() const;
        const Vector3 sum();
        bool active();
        int age();
        bool isroot() const;
        void setroot(bool val);
        bool isparent() const;
        void ang(bool grow);
        bool mark();
        const Vector3& mark() const;
        const Vector3 div();
        const Point3& mp() const;
        double radius();


        void setbranch(Point3 start, const Point3 end, int date);
        void kill();
        void computeOrientation();
        void orient(Vector3 rman, double final);
        void mark(bool set);
        void markTwo(bool set);
        void clearRadius();
        void visibility(bool set);
        static void switchSisibilityOff(Branch* b);
        const bool visibility();
        void radius(double set, int totChil);
        void sum(const Point3& end);
        void isparent(bool birth);
        vector<Branch*> children;
        Branch* me;

    private:
        bool parent;
        bool root;
        Point3 sNode;
        Point3 eNode;
        Vector3 dir;
        Vector3 summed;
        int generation;
        Point3 midpoint;
        double radi;
        double angle;
        bool alive;
        bool checkD;
        bool isVisible;
};

#endif
