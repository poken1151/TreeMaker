/*
 * leafcloud.h
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
 *      Class descriptor for the point cloud needed for SC.
 *      It holds a sub class called "Leaf" that represents the
 *      goal-nodes of our system.
 *      The class serves as an accessor to the leaf class's
 *      information.
 */
#ifndef LEAFCLOUD_H
#define LEAFCLOUD_H


#include <QtOpenGL>
#include <QDebug>
#include <vector>
#include "vecmath.h"
#include "ray.h"
#include "branch.h"

class LeafCloud {
public:
    LeafCloud();
    LeafCloud(int val, int size, int shape, double iR, double kR, double xx, double yy, double zz, double cS, int seed, int cID);
    ~LeafCloud();    

    GLUquadricObj *quadricSph;
    GLUquadricObj *quadricEli;
    GLUquadricObj *quadricShe;

    int indexVal;
    int size();
    const int remaining();
    bool inme(Point3* node);
    bool influence(Point3* node);
    void kill();
    void createCloud(Vector3 params);
    bool addLeaf(Point3 locale, double r, double in, int val);

    double getName() const;

    const Point3 location();
    int idnum();
    const int getCloudType() const;
    bool active();
    double radius();
    double cloudRad();
    double influence();
    void lifeExtend();
    bool lifeDegrade();
    const bool selection();
    void selection(bool);

    void setCloudId(int val);
    const int getCloudId();

    void changeParams(Vector4 *, Vector4 *, int);

    const Point3& getCenter();
    bool intersectionTest(Ray mouse, Point3& t);
    int checkBranch(Branch* tim);
    void influence(double set);
    void radius(double set);
    void changeCenter(Point3 *change);
    void draw(bool self, bool solid, bool leaves, bool kr, bool ir);


    LeafCloud& operator[](int idx)
    {
        indexVal = idx;
        return *this;
    }

    Vector4 getCloudRads();
    void updateAABB(Point3 *newPos);
protected:
    class Leaf {
        // Leaf class holds he values for the standard goal-nodes of the
        // tool.
        public:
            Leaf() : rad(0), influ(0), coord(Point3()), alive(true),
                sameNode(0), lastBranch(new Branch), isDrawable(true), noNode(0), pID(0) {}
            Leaf(const Point3 loca, double r, double in, int val, int pid) : coord(loca), rad(r), name((float)val/255.0),
                influ(in), pID(pid), alive(true), sameNode(0), lastBranch(new Branch), isDrawable(true) {}
            ~Leaf() {}

            const Point3& location() const {return coord;}
            bool active() {return alive;}
            void kill() {alive = false;}
            double radius() {return rad;}
            double influence() {return influ;}

            void lifeExtend() {noNode = 0;}
            bool lifeDegrade()
            {
                if(++noNode == 4)
                {
                    kill();
                    return true;
                }
                else
                    return false;

            }

            void radius(double set) {rad = set;}
            void influence(double set) {influ = set;}
            void location(const Point3 locat) {coord = locat;}

            int checkBranch(Branch* tim){
                 if(tim == lastBranch)
                     sameNode++;
                 else
                 {
                     sameNode = 0;
                     lastBranch = tim;
                 }

                 return sameNode;
             }
            void draw(Point3 loca, bool self, bool kill, bool infl, bool sel);
            int pID;

        private:
            int sameNode;
            int noNode;
            Branch* lastBranch;
            Point3 coord;
            bool alive;
            bool inCloud;
            double rad;
            double name;
            double isDrawable;
            double influ;
    };

private:
    Point3 positions[8];
    int cloudSize;
    int leavesLeft;
    int cloudType;
    double name;
    double myRad;
    int cloudID;
    bool isSelected;
    Point3 center;
    Vector4 cloudRads;
    vector<Leaf*> leaves;
};

#endif
