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
 *     Implementation file.
 */
#include "ellipsoid.h"

Ellipsoid::Ellipsoid()
{
    length = 50;
    height = 50;
    width = 10;
    axis = 0;
    isSelected = false;

    changePosition(new Point3(20, 0, 0));

    quadricEli= gluNewQuadric();
    nq = gluNewQuadric();
}

Ellipsoid::Ellipsoid(Point3 *botCor, double len, double wid, double hei, int axi)
{
    length = axi == 2 ? len : wid;
    width = axi == 0 ? len : wid;
    height = hei;
    axis = axi;
    isSelected = false;
    objType = 1;

    changePosition(botCor);
    moved = true;

    quadricEli= gluNewQuadric();
    nq = gluNewQuadric();
}

bool Ellipsoid::draw(bool self, bool wire) const
{
    if(!self)
        return false;

    if(wire)
    {
        glPushMatrix();

        if(!isSelected)
            glColor3f (0.8f, 0.8f, 1.0f);
        else
            glColor3f (1.0f, 0.2f, 0.2f);

        glLineWidth( 0.5f );
        glTranslatef(center.x, center.y, center.z);
        glScalef(width, height, length);
        glRotated(90, 1, 0, 0);

        gluQuadricDrawStyle(quadricEli, GLU_LINE);
        gluQuadricNormals(quadricEli, GLU_SMOOTH);
        gluQuadricOrientation(quadricEli, GLU_OUTSIDE);
        gluSphere(quadricEli, 1, 30, 30);

        glPopMatrix();
    }
    else
    {
        glPushMatrix();
            glEnable(GL_LIGHTING);

            if(!isSelected)
                glColor3f (0.8f, 0.8f, 1.0f);
            else
                glColor3f (1.0f, 0.2f, 0.2f);

            float specular[] = {0.2f, 0.2f, 0.2f , 0.2f};
            float ambient[] = {0.2f, 0.2f, 0.2f};
            float diffuse[] = {0.8f, 0.8f, 1.0f , 1.0f};

            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);


            glTranslatef(center.x, center.y, center.z);
            glScalef(width, height, length);

            gluQuadricNormals(quadricEli, GLU_SMOOTH);
            gluQuadricOrientation(quadricEli, GLU_OUTSIDE);
            gluSphere(quadricEli, 1, 30, 30);
            glDisable(GL_LIGHTING);
        glPopMatrix();

        glPushMatrix();

            glColor3f (0.2f, 0.2f, 0.2f);

            glLineWidth( 0.5f );
            glTranslatef(center.x, center.y, center.z);
            glScalef(width+0.1, height+0.1, length+0.1);
            glRotated(90, 1, 0, 0);

            gluQuadricDrawStyle(nq, GLU_LINE);
            gluQuadricNormals(nq, GLU_SMOOTH);
            gluQuadricOrientation(nq, GLU_OUTSIDE);
            gluSphere(nq, 1, 30, 30);
        glPopMatrix();
    }
}

bool Ellipsoid::intersectionTest(Ray &r, Point3 &t) const
{
    // AABB Intersection code based and referenced from open-code sample from codrcorner.com
    // Utilized here with my edits and in aabb.cpp and leafcloud.cpp
    int RIGHT = 0;
    int LEFT = 1;
    int MIDDLE = 2;
    bool possible = false;

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
            quadrant[i] = MIDDLE;
    }

    if(inside)
    {
        coord[0] = origin[0];
        coord[1] = origin[1];
        coord[2] = origin[2];
        possible = true;
    }

    if(!possible)
    {

        for (i = 0; i < 3; i++)
        {
            if (quadrant[i] != 2 && dir[i] !=0.)
                maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
            else
                maxT[i] = -1.;
        }

        whichPlane = 0;
        for (i = 1; i < 3; i++)
        {
            if (maxT[whichPlane] < maxT[i])
                whichPlane = i;
        }

        if (maxT[whichPlane] < 0.) return (false);
        for (i = 0; i < 3; i++)
        {
            if (whichPlane != i)
            {
                coord[i] = origin[i] + maxT[whichPlane] *dir[i];
                if (coord[i] < minB[i] || coord[i] > maxB[i])
                    return false;
            }
            else
                coord[i] = candidatePlane[i];
        }

        t.x = coord[0]; t.y = coord[1]; t.z = coord[2];
        possible = true;			/* ray hits box */
    }

    if(possible)
    {
        // I understand I can transform the sphere-coordinate space to determine
        // ray intersection for an ellipse, however I couldn't get it working, so
        // I just solved the equations... It's slow.
        Vector3 rayELlipseDIff = r.getOrigin() - Point3::PtV(center);
        Vector3 f = r.getDirection();
        f.normalize();

        float a = ((r.getDirection().x*r.getDirection().x)/(width*width))
                + ((r.getDirection().y*r.getDirection().y)/(height*height))
                + ((r.getDirection().z*r.getDirection().z)/(length*length));

        float b = ((2*rayELlipseDIff.x*r.getDirection().x)/(width*width))
                + ((2*rayELlipseDIff.y*r.getDirection().y)/(height*height))
                + ((2*rayELlipseDIff.z*r.getDirection().z)/(length*length));

        float c = ((rayELlipseDIff.x*rayELlipseDIff.x)/(width*width))
                + ((rayELlipseDIff.y*rayELlipseDIff.y)/(height*height))
                + ((rayELlipseDIff.z*rayELlipseDIff.z)/(length*length))
                - 1;

        float d = ((b*b)-(4*a*c));
        if ( d < 0 )
            return false;
        else
            d = sqrt(d);

        float hit = (-b + d)/(2*a);
        float hitsecond = (-b - d)/(2*a);

        if( hit < hitsecond) {}
        else
            hit = hitsecond;

        t = Point3::VtP(r.getOrigin() + hit * f);//r.getDirection(); /*or the normalized f*/

        return true;
    }
    else
        return false;
}

bool Ellipsoid::intersectionTest(Point3* toTest) const
{
    double test = ((pow(toTest->x - center.x, 2)) / ( width * width)  +
                   (pow(toTest->y - center.y, 2)) / (height * height) +
                   (pow(toTest->z - center.z, 2)) / (length * length));

    return (test <= 1);
}
