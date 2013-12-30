#include "octnode.h"

Octnode::Octnode() : data(0), isLeaf(true), maxDepth(1), maxPoints(100), hasChild(false), AA(Point3(0.0, 0.0, 0.0)) , BB(Point3(0.0, 0.0, 0.0))
{
    for(int i = 0; i < 8; ++i)
    {
        child[i] = NULL;
    }
    size(false);
}


Octnode::Octnode(int height, int space, int maxP) :  data(0), isLeaf(true), maxPoints(maxP), hasChild(false)
{
    for(int i = 0; i < 8; ++i)
    {
        child[i] = NULL;
    }

    double ns = (double)space/2.0;

    //This default costructor is for he very firs node. It assumes the initial AABB is centered around the origin.
    AA = Point3(-ns, -ns+height, -ns);
    BB = Point3(ns, ns+height, ns);
    centroid = Point3((AA.x+BB.x)/2, (AA.y+BB.y)/2, (AA.z+BB.z)/2);
    size(false);
}

Octnode::Octnode(Point3 iAA, Point3 iBB, int maxP) :  data(0), isLeaf(true), maxPoints(maxP), hasChild(false), AA(iAA) , BB(iBB)
{
    for(int i = 0; i < 8; ++i)
    {
        child[i] = NULL;
    }

    centroid = Point3((AA.x+BB.x)/2.0, (AA.y+BB.y)/2.0, (AA.z+BB.z)/2.0);
    size(false);
}

Octnode::~Octnode()
{

}

bool Octnode::inMe(Point3& test)
{
    bool qt = ((test.x >= AA.x && test.y >= AA.y) && test.z >= AA.z)
        && ((test.x <= BB.x && test.y <= BB.y) && test.z <= BB.z);

    bool pt = ((test.x >= BB.x && test.y >= BB.y) && test.z >= BB.z)
        && ((test.x <= AA.x && test.y <= AA.y) && test.z <= AA.z);


    if(qt || pt)
    {
        return true;
    }
    else
        return false;
}

bool Octnode::inMe(Branch& newBranch)
{
    Point3 test = newBranch.node();
    bool qt = ((test.x >= AA.x && test.y >= AA.y) && test.z >= AA.z)
        && ((test.x <= BB.x && test.y <= BB.y) && test.z <= BB.z);

    bool pt = ((test.x >= BB.x && test.y >= BB.y) && test.z >= BB.z)
        && ((test.x <= AA.x && test.y <= AA.y) && test.z <= AA.z);


    if(qt || pt)
    {
        return true;
    }
    else
        return false;
}

bool Octnode::push(Branch* newBranch)
{
    size(true);
    //Point3 t = (*newBranch).node();
    Point3 iAA, iBB;
    if((myPoints.size() < maxPoints) && !hasChild)
    {
        myPoints.push_back(newBranch);
        return true;
    }
    else if(hasChild)
    {
        for(int j = 0; j < 8; ++j)
        {
            if((*child[j]).inMe((*newBranch)))
            {
                (*child[j]).push(newBranch);
                break;
            }
        }
        return true;
    }

    double len = (Point3(AA.x, 0.0, 0.0).distanceTo(Point3(BB.x, 0.0, 0.0))) / 2.0;

    iAA = AA;
    iBB = centroid;
    child[0] = new Octnode(iAA, iBB, maxPoints);

    iAA.z += len;
    iBB.z += len;
    child[1] = new Octnode(iAA, iBB, maxPoints);

    iAA = AA;
    iBB = centroid;
    iAA.y += len;
    iBB.y += len;
    child[2] = new Octnode(iAA, iBB, maxPoints);

    iAA.z += len;
    iBB.z += len;
    child[3] = new Octnode(iAA, iBB, maxPoints);

    iAA = AA;
    iBB = centroid;
    iAA.x += len;
    iBB.x += len;
    child[4] = new Octnode(iAA, iBB, maxPoints);

    iAA.z += len;
    iBB.z += len;
    child[5] = new Octnode(iAA, iBB, maxPoints);

    iAA = AA;
    iBB = centroid;
    iAA.x += len;
    iBB.x += len;
    iAA.y += len;
    iBB.y += len;
    child[6] = new Octnode(iAA, iBB, maxPoints);

    iAA.z += len;
    iBB.z += len;
    child[7] = new Octnode(iAA, iBB, maxPoints);

    for(int i = 0; i < myPoints.size(); ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            if(child[j]->inMe((*myPoints[i])))
            {
                child[j]->push(myPoints[i]);
                break;
            }
        }
    }

    for(int j = 0; j < 8; ++j)
    {
        if(child[j]->inMe((*newBranch)))
        {
            child[j]->push(newBranch);
            break;
        }
    }

    isLeaf = false;
    hasChild = true;

    myPoints.clear();

    return true;
}
void Octnode::size(bool t)
{
    if(t)
        s++;
    else
        s = 0;
}

int Octnode::size() const
{
    return s;
}

Point3 Octnode::A() const
{
    return AA;
}

Point3 Octnode::B() const
{
    return BB;
}
