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
 *      Implementation File
 */
#include "ray.h"

Ray::Ray() {
    setDirection(Vector3(0, 0, -1));
    setInvDirection(Vector3(0, 0, -1));
    setOrigin(Vector3(0, 0, 0));
}

Ray::Ray(const Vector3 origin, const Vector3 direc) {
    setDirection(direc);
    setOrigin(origin);
    setInvDirection(direc);
}

Ray::Ray(const Point3 origin, const Point3 end) {
    Vector3 dirc = end - origin;

    setDirection(dirc); //Vector3(origin, end)
    setInvDirection(dirc);
    setOrigin(Vector3(origin.x, origin.y, origin.z));
}

Vector3 Ray::planeIntersect()
{
    float s   = -getOrigin().z / getDirection().z;
    Vector3 worldPos(0, 0, 0);
    worldPos.x = getOrigin().x + getDirection().x*s;
    worldPos.y = getOrigin().y + getDirection().y*s;
    worldPos.z = 0;

    return worldPos;
}

const Vector3& Ray::getInvDirection() const{
    return inv_direction;
}

const Vector3& Ray::getDirection() const{
    return direction;
}

const Vector3& Ray::getOrigin() const{
    return origin;
}

void Ray::setInvDirection(Vector3 direc) {
    inv_direction = Vector3(1.0/direc.x, 1.0/direc.y, 1.0/direc.z);

    sign[0] = (inv_direction.x < 0) ? 1 : 0;
    sign[1] = (inv_direction.y < 0) ? 1 : 0;
    sign[2] = (inv_direction.z < 0) ? 1 : 0;
}

void Ray::setDirection(Vector3 direc) {
    direc.normalize();
    direction = direc;
    setInvDirection(direc);
}

void Ray::setOrigin(Vector3 p) {
    origin = p;
}

void Ray::setOrigin(Point3 p) {
    origin = Vector3(p.x, p.y, p.z);
}
