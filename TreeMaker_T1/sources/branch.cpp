/*
 * branch.cpp
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
 *     Implementation file for branch.h.
 */
#include "branch.h"

Branch::Branch()
{
    root = false;
    radi = 0.0;
    isVisible = true;
}

Branch::Branch(Point3* start, Point3* end, int date)
{
    setroot(false);
    sNode = (*start);
    eNode = (*end);

    startNodeTest = start;
    endNodeTest = end;

    computeOrientation();

    dir = eNode - sNode;
    summed = dir;
    //midpoint = Point3((start.x+end.x)/2, (start.y+end.y)/2, (start.z+end.z)/2);
    checkD = false;
    alive = true;
    parent = false;
    radi = 0.0;
    generation = date;
    angle = 0;
    spawn = 0;
    isVisible = true;
    //lookUp = lu;
}


Branch::Branch(const Point3 start, const Point3 end, int date, int lu)
{
    setroot(false);
    computeOrientation();
    sNode = start;
    eNode = end;
    dir = eNode - sNode;
    summed = dir;
    //midpoint = Point3((start.x+end.x)/2, (start.y+end.y)/2, (start.z+end.z)/2);
    checkD = false;
    alive = true;
    parent = false;
    radi = 0.0;
    generation = date;
    angle = 0;
    spawn = 0;
    isVisible = true;
    //lookUp = lu;
}

Branch::Branch(const Point3 start, const Vector3 way, int date, int lu)
{
    setroot(false);
    computeOrientation();
    sNode = start;
    eNode = Point3(start.x+way.x, start.y+way.y, start.z+way.z);
    dir = way;
    summed = way;
    //midpoint = Point3((start.x+eNode.x)/2, (start.y+eNode.y)/2, (start.z+eNode.z)/2);
    checkD = false;
    alive = true;
    parent = false;
    root = false;
    radi = 0.0;
    generation = date;
    angle = 0;
    spawn = 0;
    isVisible = true;
    //lookUp = lu;
 }

void Branch::ang(bool grow) {
    if(grow)
        angle++;
    else if(!grow)
        angle = 0;
}


Branch::~Branch()
{
    //    for(int i = 0; i < children.size(); ++i)
    //    {
    //        Branch *temp = children[i];
    //        delete temp;
    //    }
    //    delete startNodeTest;
    //    delete endNodeTest;
    //    children.clear();
}

void Branch::switchSisibilityOff(Branch* b)
{
    vector<Branch*> stack;
    Branch* c;

    stack.push_back(b);
    while(!stack.empty())
    {
        c = stack.back();
        c->markTwo(false);
        stack.erase(stack.end() - 1);

        c->visibility(false);
        for(int i = 0; i < c->children.size(); ++i)
        {
            stack.push_back(c->children[i]);
        }
    }
}

void Branch::computeOrientation()
{
    Vector3 a(endNodeTest->x, endNodeTest->y, endNodeTest->z);
    Vector3 b(startNodeTest->x, startNodeTest->y, startNodeTest->z); //(the two points you want to draw between)

    // This is the default direction for the cylinders to face in OpenGL
    Vector3 z = Vector3(0,0,1);
    // Get diff between two points you want cylinder along
    Vector3 p = (a - b); p.normalize();
    // Get CROSS product (the axis of rotation)
    Vector3 t = z^p; t.normalize();

    // Get angle. LENGTH is magnitude of the vector
    double angle = 180 / M_PI * acos(z*p / p.length());

    orient(t, angle);
}


void Branch::orient(Vector3 rman, double final)
{
    rot = rman;
    fangle = final;
}

bool Branch::isroot() const
{
    return root;
}

void Branch::setroot(bool val)
{
    root = val;;
}

bool Branch::isparent() const
{
    return parent;
}
void Branch::isparent(bool birth)
{
    parent = birth;
}

const Point3& Branch::node() const
{
    return eNode;
}

void Branch::visibility(bool set)
{
    isVisible = set;
}

const bool Branch::visibility()
{
    return isVisible;
}

const Point3& Branch::middle() const
{
    return sNode;
}

const Point3& Branch::mp() const
{
    return midpoint;
}

const Vector3& Branch::direction() const
{
    return dir;
}

bool Branch::active()
{
    return alive;
}

int Branch::age()
{
    return generation;
}

void Branch::clearRadius()
{
    radi = 0.0;
}

void Branch::radius(double set, int totChil)
{
    radi +=  set * set * ((totChil+0.0000001)/(1000 - totChil));
    //radi += set * set;
    //cout << radi << endl;
    //radi = set;
}

double Branch::radius()
{
    return radi;
}

void Branch::mark(bool set)
{
    checkD = set;
    spawn++;
}

void Branch::markTwo(bool set)
{
    checkD = set;
}

void Branch::sum(const Point3& end)
{
    Vector3 temp = end - eNode;
    temp = temp/(eNode.distanceTo(end));
    //temp.normalize();
    summed += temp;
    //summed.normalize();
    //summed.x -= (summed.x*0.7);
    //summed.y -= (summed.y*0.7);
    //summed.z -= (summed.z*0.7);
}

const Vector3 Branch::div()
{
    /*
      summed.x = summed.x/angle; (summed.x*0.97);
      summed.y = summed.y/angle; (summed.y*0.97);
      summed.z = summed.z/angle; (summed.z*0.97);*/
    summed = summed/angle;
    summed.normalize();
    parent = true;
    return summed;
}

const Vector3 Branch::sum()
{
    return summed;
}

bool Branch::mark()
{
    return checkD;
}

void Branch::setbranch(const Point3 start, const Point3 end, int date)
{
    sNode = start;
    eNode = end;
    dir = eNode - sNode;
    summed = dir;
    //summed = dir * date;
    //midpoint = Point3((start.x+end.x)/2, (start.y+end.y)/2, (start.z+end.z)/2);
    checkD = false;
    alive = true;
    radi = 0.0;
    generation = date;
    parent = false;
    // generation++;
    angle = 0;
}

void Branch::kill()
{
    alive = false;
}
