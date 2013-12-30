/*
 * aabb.cpp
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
 *     Implementation file for aabb.h.
 */
#include "aabb.h"

AABB::AABB()
{
    length = 50;
    height = 50;
    width = 10;
    axis = 0;
    isSelected = false;

    changePosition(new Point3(20, 0, 0));
}

AABB::AABB(Point3 *botCor, double len, double wid, double hei, int axi)
{
    length = axi == 2 ? len : wid;
    width = axi == 0 ? len : wid;
    height = hei;
    axis = axi;
    isSelected = false;
    objType = 0;

    changePosition(botCor);
    moved = true;
}

AABB::AABB(Point3 *botCor, int axi)
{
    axis = axi;
    isSelected = false;

    switch(axis)
    {
    case 0:
        width = 25;
        height = 3;
        length = 3;
        break;
    case 1:
        width = 3;
        height = 25;
        length = 3;
        break;
    case 2:
        width = 3;
        height = 3;
        length = 25;
        break;
    }

    changePosition(botCor);
    moved = true;
    objType = 2;
}

bool AABB::draw(bool self, bool wire) const
{
    if(!self)
        return false;

    glPushMatrix();

    if(!isSelected)
        glColor3f (0.8f, 0.8f, 1.0f);
    else
        glColor3f (1.0f, 0.2f, 0.2f);

    if(!wire)
    {
        glBegin(GL_QUADS);
            //LEFT AND RIGHT
            glVertex3f(positions[0].x, positions[0].y, positions[0].z);
            glVertex3f(positions[1].x, positions[1].y, positions[1].z);
            glVertex3f(positions[2].x, positions[2].y, positions[2].z);
            glVertex3f(positions[3].x, positions[3].y, positions[3].z);

            glVertex3f(positions[4].x, positions[4].y, positions[4].z);
            glVertex3f(positions[7].x, positions[7].y, positions[7].z);
            glVertex3f(positions[6].x, positions[6].y, positions[6].z);
            glVertex3f(positions[5].x, positions[5].y, positions[5].z);

            //BACK AND FRONT
            glVertex3f(positions[0].x, positions[0].y, positions[0].z);
            glVertex3f(positions[3].x, positions[3].y, positions[3].z);
            glVertex3f(positions[7].x, positions[7].y, positions[7].z);
            glVertex3f(positions[4].x, positions[4].y, positions[4].z);

            glVertex3f(positions[1].x, positions[1].y, positions[1].z);
            glVertex3f(positions[5].x, positions[5].y, positions[5].z);
            glVertex3f(positions[6].x, positions[6].y, positions[6].z);
            glVertex3f(positions[2].x, positions[2].y, positions[2].z);

            //TOP AND BOTTOM
            glVertex3f(positions[0].x, positions[0].y, positions[0].z);
            glVertex3f(positions[4].x, positions[4].y, positions[4].z);
            glVertex3f(positions[5].x, positions[5].y, positions[5].z);
            glVertex3f(positions[1].x, positions[1].y, positions[1].z);

            glVertex3f(positions[3].x, positions[3].y, positions[3].z);
            glVertex3f(positions[2].x, positions[2].y, positions[2].z);
            glVertex3f(positions[6].x, positions[6].y, positions[6].z);
            glVertex3f(positions[7].x, positions[7].y, positions[7].z);
        glEnd();
    }

    glLineWidth( 2.5f );

    glBegin(GL_LINES);
        glColor3f(0.2f, 0.2f, 0.4f);
        glEnable( GL_LINE_SMOOTH );

        glVertex3f(positions[0].x, positions[0].y, positions[0].z);
        glVertex3f(positions[1].x, positions[1].y, positions[1].z);

        glVertex3f(positions[0].x, positions[0].y, positions[0].z);
        glVertex3f(positions[4].x, positions[4].y, positions[4].z);

        glVertex3f(positions[0].x, positions[0].y, positions[0].z);
        glVertex3f(positions[3].x, positions[3].y, positions[3].z);

        glVertex3f(positions[2].x, positions[2].y, positions[2].z);
        glVertex3f(positions[1].x, positions[1].y, positions[1].z);

        glVertex3f(positions[2].x, positions[2].y, positions[2].z);
        glVertex3f(positions[3].x, positions[3].y, positions[3].z);

        glVertex3f(positions[2].x, positions[2].y, positions[2].z);
        glVertex3f(positions[6].x, positions[6].y, positions[6].z);

        glVertex3f(positions[5].x, positions[5].y, positions[5].z);
        glVertex3f(positions[6].x, positions[6].y, positions[6].z);

        glVertex3f(positions[5].x, positions[5].y, positions[5].z);
        glVertex3f(positions[1].x, positions[1].y, positions[1].z);

        glVertex3f(positions[5].x, positions[5].y, positions[5].z);
        glVertex3f(positions[4].x, positions[4].y, positions[4].z);

        glVertex3f(positions[7].x, positions[7].y, positions[7].z);
        glVertex3f(positions[3].x, positions[3].y, positions[3].z);

        glVertex3f(positions[7].x, positions[7].y, positions[7].z);
        glVertex3f(positions[6].x, positions[6].y, positions[6].z);

        glVertex3f(positions[7].x, positions[7].y, positions[7].z);
        glVertex3f(positions[4].x, positions[4].y, positions[4].z);
    glEnd();

    glPopMatrix();
}

bool AABB::intersectionTest(Ray &r, Point3 &t) const
{
    // AABB Intersection code based on and referenced from open-code sample from codrcorner.com
    // Utilized here with my edits and in leafcloud.cpp and ellipsoid.cpp
    int RIGHT = 0;
    int LEFT = 1;
    int MIDDLE = 2;

    double minB[3] = {positions[0].x, positions[0].y, positions[0].z};
    double maxB[3] = {positions[6].x, positions[6].y, positions[6].z};
    double origin[3] = {r.getOrigin().x, r.getOrigin().y, r.getOrigin().z};
    double dir[3] = {r.getDirection().x, r.getDirection().y, r.getDirection().z};
    double coord[3];

    char inside = true;
    char quadrant[3];
    register int i;
    int whichPlane;
    double maxT[3];
    double candidatePlane[3];

    for (i=0; i<3; i++)
    {
        if(origin[i] < minB[i])
        {
            quadrant[i] = LEFT;
            candidatePlane[i] = minB[i];
            inside = false;
        }
        else if(origin[i] > maxB[i])
        {
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxB[i];
            inside = false;
        }
        else
        {
            quadrant[i] = MIDDLE;
        }
    }

    if(inside)
    {
        coord[0] = origin[0];
        coord[1] = origin[1];
        coord[2] = origin[2];
        return true;
    }

    for (i = 0; i < 3; i++)
    {
        if(quadrant[i] != 2 && dir[i] !=0.)
            maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
        else
            maxT[i] = -1.;
    }

    whichPlane = 0;
    for (i = 1; i < 3; i++)
    {
        if(maxT[whichPlane] < maxT[i])
            whichPlane = i;
    }

    if(maxT[whichPlane] < 0.) return (false);

    for (i = 0; i < 3; i++)
    {
        if(whichPlane != i)
        {
            coord[i] = origin[i] + maxT[whichPlane] *dir[i];
            if(coord[i] < minB[i] || coord[i] > maxB[i])
                return (false);
        }
        else
            coord[i] = candidatePlane[i];
    }

    t.x = coord[0]; t.y = coord[1]; t.z = coord[2];

    return true;
}

bool AABB::intersectionTest(Point3* toTest) const
{
    double minB[3] = {positions[0].x - 5, positions[0].y - 5, positions[0].z - 5};
    double maxB[3] = {positions[6].x + 5, positions[6].y + 5, positions[6].z + 5};
    double origin[3] = {toTest->x, toTest->y, toTest->z};

    char inside = true;
    char quadrant[3];
    register int i;
    double candidatePlane[3];

    /* Find candidate planes; this loop can be avoided if
     * rays cast all from the eye(assume perpsective view) */
    for (i=0; i<3; i++)
    {
        if(origin[i] < minB[i])
        {
            quadrant[i] = 1;
            candidatePlane[i] = minB[i];
            inside = false;
        }
        else if (origin[i] > maxB[i])
        {
            quadrant[i] = 0;
            candidatePlane[i] = maxB[i];
            inside = false;
        }
        else
            quadrant[i] = 2;
    }

    /* Ray origin inside bounding box */
    if(inside)
        return (true);

    return false;
}

void AABB::drawGiz(bool move, bool scale)
{
    glPushMatrix();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_DEPTH);

        if(move || scale)
        {
            glLineWidth( 4.0f );
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.9f, 0.0f);
            glEnable( GL_LINE_SMOOTH );

            switch(axis)
            {
            case 0:
                glColor3f(0.9f, 0.0f, 0.0f);
                glVertex3f(center.x, center.y, center.z);
                glVertex3f(center.x + width, center.y, center.z);
                break;
            case 1:
                glColor3f(0.0f, 0.9f, 0.0f);
                glVertex3f(center.x, center.y, center.z);
                glVertex3f(center.x, center.y + height, center.z);
                break;
            case 2:
                glColor3f(0.0f, 0.0f, 0.9f);
                glVertex3f(center.x, center.y, center.z);
                glVertex3f(center.x, center.y, center.z + length);
                break;
            }

            glEnd();
        }

        glEnable(GL_DEPTH);
        glEnable(GL_DEPTH_TEST);

    glPopMatrix();
}
