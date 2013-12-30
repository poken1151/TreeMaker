/*
 * ray.h
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
 *      Class descriptor for a standard Ray class used for ray-casting.
 */
#ifndef RAY_H
#define RAY_H

#include "vecmath.h"

class Ray {
private:
    Vector3 origin;
    Vector3 direction;
    Vector3 inv_direction;

public:
    Ray();
    Ray(const Vector3 origin, const Vector3 direc);
    Ray(const Point3 origin, const Point3 end);
    Vector3 planeIntersect();

    const Vector3& getInvDirection() const;
    const Vector3& getDirection() const;
    const Vector3& getOrigin() const;

    void setInvDirection(Vector3 direc);
    void setDirection(Vector3 direc);
    void setOrigin(Vector3 startPoint);
    void setOrigin(Point3 startPoint);

    int sign[3];
};

#endif
