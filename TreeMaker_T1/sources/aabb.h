/*
 * aabb.h
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
 *     This file extends object.h. It bulds a standard aabb.
 */

#ifndef AABB_H
#define AABB_H

#include "object.h"

class AABB : public Object {
 public:
    AABB();
    AABB(Point3* botCor, double len, double wid, double brd, int axi);
    AABB(Point3* botCor, int axi);

    virtual bool draw(bool self, bool solid) const;
    virtual bool intersectionTest(Ray& r, Point3& t) const;
    virtual bool intersectionTest(Point3 *toTest) const;

    void drawGiz(bool move, bool scale);
 };

 #endif
