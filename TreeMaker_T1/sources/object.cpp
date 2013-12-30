/*
 * object.cpp
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
 *      Implementation file.
 */
#include "object.h"
#include <vector>

Object::Object() {}

Object::~Object() {}

bool Object::changePosition(Point3 *newPos)
{
    center = *newPos;
    switch(objType)
    {
    case 0:
        positions[0] = Point3(newPos->x - (width / 2), newPos->y - (height / 2), newPos->z - (length / 2));
        positions[1] = Point3(positions[0].x, positions[0].y, positions[0].z + length);
        positions[2] = Point3(positions[0].x, positions[0].y + height, positions[0].z + length);
        positions[3] = Point3(positions[0].x, positions[0].y + height, positions[0].z);
        positions[4] = Point3(positions[0].x + width, positions[0].y, positions[0].z);
        positions[5] = Point3(positions[0].x + width, positions[0].y, positions[0].z + length);
        positions[6] = Point3(positions[0].x + width, positions[0].y + height, positions[0].z + length);
        positions[7] = Point3(positions[0].x + width, positions[0].y + height, positions[0].z);
        break;
    case 1:
        positions[0] = Point3(newPos->x - width, newPos->y - height, newPos->z - length);
        positions[1] = Point3(positions[0].x, positions[0].y, positions[0].z + length*2);
        positions[2] = Point3(positions[0].x, positions[0].y + height*2, positions[0].z + length*2);
        positions[3] = Point3(positions[0].x, positions[0].y + height*2, positions[0].z);
        positions[4] = Point3(positions[0].x + width*2, positions[0].y, positions[0].z);
        positions[5] = Point3(positions[0].x + width*2, positions[0].y, positions[0].z + length*2);
        positions[6] = Point3(positions[0].x + width*2, positions[0].y + height*2, positions[0].z + length*2);
        positions[7] = Point3(positions[0].x + width*2, positions[0].y + height*2, positions[0].z);
        break;
    case 2:
        positions[0] = Point3(center.x - 1.5, center.y - 1.5, center.z - 1.5);    positions[1] = Point3(positions[0].x, positions[0].y, positions[0].z + length);
        positions[2] = Point3(positions[0].x, positions[0].y + height, positions[0].z + length);
        positions[3] = Point3(positions[0].x, positions[0].y + height, positions[0].z);
        positions[4] = Point3(positions[0].x + width, positions[0].y, positions[0].z);
        positions[5] = Point3(positions[0].x + width, positions[0].y, positions[0].z + length);
        positions[6] = Point3(positions[0].x + width, positions[0].y + height, positions[0].z + length);
        positions[7] = Point3(positions[0].x + width, positions[0].y + height, positions[0].z);
        break;
    }

    moved = true;
    return true;
}

Point3 &Object::getPosition()
{
    return center;
}

Vector4 Object::getSize()
{
    return Vector4(length, width, height, -1);
}

void Object::changeSize(Vector4 *newSize)
{
    length = newSize->x;
    width =  newSize->y;
    height =  newSize->z;
    changePosition(&center);
}

bool Object::hasMoved()
{
    bool m = false;

    if(moved)
    {
        m = true;
        moved = false;
    }

    return m;
}

void Object::selection(bool val)
{
    isSelected = val;
}

const bool Object::selection()
{
    return isSelected;
}


