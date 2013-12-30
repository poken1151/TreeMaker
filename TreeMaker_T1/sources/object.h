/*
 * object.h
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
 *      Base class for our aabb and ellipsoid objects.
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vecmath.h"

class Object {
 public:
  Object();
  virtual ~Object();

  virtual bool draw(bool self, bool solid) const=0;
  virtual bool intersectionTest(Ray& r, Point3& t) const=0;
  virtual bool intersectionTest(Point3 *toTest) const=0;

  Point3 &getPosition();
  Vector4 getSize();
  void changeSize(Vector4 *newSize);
  bool changePosition(Point3 *newPos);
  bool hasMoved();
  void selection(bool val);
  const bool selection();

protected:
  Point3 positions[8];
  Point3 center;
  double length;
  double width;
  double height;
  int axis;

  bool moved;
  bool isSelected;
  int objType;
 };

 #endif
