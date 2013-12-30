/*
 * ellipsoid.h
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
 *     This file extends object.h. It bulds a ellipsoid in 3D space.
 */
#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "object.h"

class Ellipsoid : public Object {
 public:
    Ellipsoid();
    Ellipsoid(Point3* botCor, double len, double wid, double brd, int axi);

    virtual bool draw(bool self, bool solid) const;
    virtual bool intersectionTest(Ray& r, Point3& t) const;
    virtual bool intersectionTest(Point3 *toTest) const;

    void drawGiz(bool move, bool scale);

protected:
    GLUquadricObj *quadricEli;
    GLUquadricObj *nq;
 };

#endif // Ellipsoid_H
