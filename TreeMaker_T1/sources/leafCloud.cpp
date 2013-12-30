/*
 * leafcloud.cpp
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
 *      Implementation File.
 */
#include "leafCloud.h"

LeafCloud::LeafCloud()
{
    name = -1;
    isSelected = false;
}

LeafCloud::LeafCloud(int val, int numOfLeaves, int shape, double iR, double kR, double xx, double yy, double zz, double cS, int seed, int cID)
{
    cloudSize = numOfLeaves;
    leavesLeft = numOfLeaves;

    name = val/255.0;
    setCloudId(cID);
    isSelected = false;

//    double rx = cS/xx;
//    double ry = cS/yy;
//    double rz = cS/zz;
    cloudType = shape;
    cloudRads = Vector4(xx, yy, zz, cS);//rx/2.0, ry/2.0, rz/2.0, cS/3.0);

    quadricSph = gluNewQuadric();
    quadricEli = gluNewQuadric();
    quadricShe = gluNewQuadric();

    createCloud(Vector3(kR, iR, seed));
    updateAABB(new Point3(0, 0, 0));
}

LeafCloud::~LeafCloud()
{    
    //qDebug() << "Cloud deleted\n";

    for(int i = 0; i < leaves.size(); ++i)
    {
        delete leaves[i];
    }

    leaves.clear();
}

Vector4 LeafCloud::getCloudRads()
{
    return cloudRads;
}

bool LeafCloud::intersectionTest(Ray r, Point3& t)
{
    if(cloudType == 3) //Same from AABB
    {
        // AABB Intersection code based and referenced from open-code sample from codrcorner.com
        // Utilized here with my edits and in aabb.cpp and ellipsoid.cpp
        int RIGHT = 0;
        int LEFT = 1;
        int MIDDLE = 2;

        double minB[3] = {getCenter().x - cloudRads.x, getCenter().y - cloudRads.y, getCenter().z - cloudRads.z};
        double maxB[3] = {getCenter().x + cloudRads.x, getCenter().y + cloudRads.y, getCenter().z + cloudRads.z};
        double origin[3] = {r.getOrigin().x, r.getOrigin().y, r.getOrigin().z};
        double dir[3] = {r.getDirection().x, r.getDirection().y, r.getDirection().z};		/*ray */
        double coord[3];				/* hit point */

            char inside = true;
            char quadrant[3];
            register int i;
            int whichPlane;
            double maxT[3];
            double candidatePlane[3];

            for (i=0; i<3; i++)
                if(origin[i] < minB[i])
                {
                    quadrant[i] = LEFT;
                    candidatePlane[i] = minB[i];
                    inside = false;
                }
                else if (origin[i] > maxB[i])
                {
                    quadrant[i] = RIGHT;
                    candidatePlane[i] = maxB[i];
                    inside = false;
                }
                else
                {
                    quadrant[i] = MIDDLE;
                }

            if(inside)
            {
                coord[0] = origin[0];
                coord[1] = origin[1];
                coord[2] = origin[2];
                return (true);
            }

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
                        return (false);
                }
                else
                    coord[i] = candidatePlane[i];
            }

            t.x = coord[0]; t.y = coord[1]; t.z = coord[2];
            return (true);				/* ray hits box */
    }
    else if(cloudType == 1)
    {
        Vector3 P = Point3::PtV(getCenter()) - r.getOrigin();

        //double A = (pow(r.getDirection().x, 2) + pow(r.getDirection().z, 2) - (0.25 * pow(r.getDirection().y, 2)));
        //double B = ((2.0 * P.x * r.getDirection().x) + (2.0 * P.z * r.getDirection().z) - (0.5 * P.y * r.getDirection().y) + (0.25 * r.getDirection().y));
        //double C = (pow(P.x, 2) + pow(P.z, 2) - (0.25 * pow(P.y, 2)) + (0.25 * P.y) - 0.0625);

        // Works, but only on infinite cone along given axis with point
        // at the origin.
        // http://everything2.com/title/Ray+Tracing%253A+intersections+with+cones
//        double A = (pow(r.getDirection().x, 2) + pow(r.getDirection().z, 2) - pow(r.getDirection().y, 2));
//        double B = ((2.0 * r.getOrigin().x * r.getDirection().x) + (2.0 * r.getOrigin().z * r.getDirection().z) - (2.0 * r.getOrigin().y * r.getDirection().y));
//        double C = (pow(r.getOrigin().x, 2) + pow(r.getOrigin().z, 2) - pow(r.getOrigin().y, 2));

        // My own attempt...
//        double h = 2 * (getCenter().y + cloudRads.y);
//        double rh = pow(cloudRads.x, 2)/pow(h, 2);
//        double A = (pow(r.getDirection().x, 2) + pow(r.getDirection().z, 2) - (rh * pow(r.getDirection().y, 2)));
//        double B = ((2.0 * r.getOrigin().x * r.getDirection().x) + (2.0 * r.getOrigin().z * r.getDirection().z) + (rh * ((2.0 * r.getDirection().y * h) - (2.0 * r.getOrigin().y * r.getDirection().y))));
//        double C = (pow(r.getOrigin().x, 2) + pow(r.getOrigin().z, 2) - rh * (pow(r.getOrigin().y, 2) - (2.0 * r.getOrigin().y * h) + pow(h, 2)));

        // New Attempt from cached site
        Vector3 coneCenter = Point3::PtV(getCenter());
        coneCenter.y -= cloudRads.y;

        double finalPP = DM;
        double h = 2 * (cloudRads.y);
        P.y = coneCenter.y - r.getOrigin().y + h;
        double E = -pow(cloudRads.x, 2)/pow(h, 2);
        double A = (pow(r.getDirection().x, 2) + (E * pow(r.getDirection().y, 2)) + pow(r.getDirection().z, 2));
        double B = P.x * r.getDirection().x + E * P.y * r.getDirection().y + P.z * r.getDirection().z;
        double C = pow(P.x, 2) + E * pow(P.y, 2) + pow(P.z, 2);

        double disc = pow(B, 2) - (/*4 * */A * C);

        if(disc < 0)
        {
//            qDebug() << "No hit...";
            return false;
        }

        double D = sqrt(disc);
        double pp1 = (B + D) / A;
        double pp2 = (B - D) / A;

        if(pp2 > 0)
        {
            if(pp1 < 0)
            {
                if(pp2 < finalPP)
                    finalPP = pp2;
            }
            else
            {
                if(pp1 < finalPP)
                    finalPP = pp1;
            }
        }

        t = Point3::VtP(r.getOrigin() + (r.getDirection() * finalPP));

        if(t.y > (getCenter().y + cloudRads.y) || t.y < (getCenter().y - cloudRads.y))
            return false;
//        qDebug() << "Hit!";
        return true;
    }
    else if(cloudType == 6)
    {
        Vector3 P = Point3::PtV(getCenter()) - r.getOrigin();

        Vector3 coneCenter = Point3::PtV(getCenter());
        coneCenter.y -= 3 * cloudRads.y;

        double finalPP = DM;
        double h = 2 * (cloudRads.y);
        P.y = coneCenter.y - r.getOrigin().y + h;
        double E = -pow(cloudRads.x, 2)/pow(h, 2);
        double A = (pow(r.getDirection().x, 2) + (E * pow(r.getDirection().y, 2)) + pow(r.getDirection().z, 2));
        double B = P.x * r.getDirection().x + E * P.y * r.getDirection().y + P.z * r.getDirection().z;
        double C = pow(P.x, 2) + E * pow(P.y, 2) + pow(P.z, 2);

        double disc = pow(B, 2) - (/*4 * */A * C);

        if(disc < 0)
        {
//            qDebug() << "No hit...";
            return false;
        }

        double D = sqrt(disc);
        double pp1 = (B + D) / A;
        double pp2 = (B - D) / A;

        if(pp2 > 0)
        {
            if(pp1 < 0)
            {
                if(pp2 < finalPP)
                    finalPP = pp2;
            }
            else
            {
                if(pp1 < finalPP)
                    finalPP = pp1;
            }
        }

        t = Point3::VtP(r.getOrigin() + (r.getDirection() * finalPP));

        if(t.y > (getCenter().y + cloudRads.y) || t.y < (getCenter().y - cloudRads.y))
            return false;
//        qDebug() << "Hit!";
        return true;
    }
    else if(cloudType == 2)
    {
        Vector3 rayELlipseDIff = r.getOrigin() - Point3::PtV(getCenter());
        Vector3 f = r.getDirection();
        f.normalize();

        float a = ((r.getDirection().x*r.getDirection().x)/(cloudRads.x*cloudRads.x))
                + ((r.getDirection().y*r.getDirection().y)/(cloudRads.y*cloudRads.y))
                + ((r.getDirection().z*r.getDirection().z)/(cloudRads.z*cloudRads.z));

        float b = ((2*rayELlipseDIff.x*r.getDirection().x)/(cloudRads.x*cloudRads.x))
                + ((2*rayELlipseDIff.y*r.getDirection().y)/(cloudRads.y*cloudRads.y))
                + ((2*rayELlipseDIff.z*r.getDirection().z)/(cloudRads.z*cloudRads.z));

        float c = ((rayELlipseDIff.x*rayELlipseDIff.x)/(cloudRads.x*cloudRads.x))
                + ((rayELlipseDIff.y*rayELlipseDIff.y)/(cloudRads.y*cloudRads.y))
                + ((rayELlipseDIff.z*rayELlipseDIff.z)/(cloudRads.z*cloudRads.z))
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
    else if(cloudType == 4 || cloudType == 5)
    {
        Vector3 rayELlipseDIff = r.getOrigin() - Point3::PtV(getCenter());
        Vector3 f = r.getDirection();
        f.normalize();

        float a = ((r.getDirection().x*r.getDirection().x)/(cloudRads.x*cloudRads.x))
                + ((r.getDirection().y*r.getDirection().y)/(cloudRads.y*cloudRads.y))
                + ((r.getDirection().z*r.getDirection().z)/(cloudRads.z*cloudRads.z));

        float b = ((2*rayELlipseDIff.x*r.getDirection().x)/(cloudRads.x*cloudRads.x))
                + ((2*rayELlipseDIff.y*r.getDirection().y)/(cloudRads.y*cloudRads.y))
                + ((2*rayELlipseDIff.z*r.getDirection().z)/(cloudRads.z*cloudRads.z));

        float c = ((rayELlipseDIff.x*rayELlipseDIff.x)/(cloudRads.x*cloudRads.x))
                + ((rayELlipseDIff.y*rayELlipseDIff.y)/(cloudRads.y*cloudRads.y))
                + ((rayELlipseDIff.z*rayELlipseDIff.z)/(cloudRads.z*cloudRads.z))
                - 1;

        float d = ((b*b)-(4*a*c));
        if ( d < 0 )
            return false;
        else
            d = sqrt(d);
        float hit = (-b + d)/(2*a);
        float hitsecond = (-b - d)/(2*a);

        Vector3 trial = r.getOrigin() + f * hit;
        Vector3 trial2 = r.getOrigin() + f * hitsecond;


        if( hit < hitsecond)
            t = Point3::VtP(trial);
        else
            t = Point3::VtP(trial2);

        if((trial.y <= getCenter().y && trial2.y <= getCenter().y && cloudType == 4))
            return false;
        if((trial.y >= getCenter().y && trial2.y >= getCenter().y && cloudType == 5))
            return false;
        else
        {}

        return true;
    }
    if(cloudType == 0)
    {
        Vector3 k = r.getOrigin();
        Point3 temp = Point3::VtP(k);
        Vector3 sphereToRay = temp - getCenter();
        Vector3 f = r.getDirection();
        f.normalize();

        float a = f * f;
        float b = (2 * f) * sphereToRay;
        float c = sphereToRay * sphereToRay - (cloudRads.x * cloudRads.x);

        float disc = b * b - 4  * a * c;

        if (disc < 0){
            return false;
        }

        float distSqrt = sqrt(disc);
        float q;
        if (b < 0)
            q = (-b - distSqrt)/2.0;
        else
            q = (-b + distSqrt)/2.0;

        float t0 = q / a;
        float t1 = c / q;

        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }

        if (t1 < 0)
            return false;

        if (t0 < 0) {
            k = r.getOrigin() + t1 * f;//r.getDirection(); /*or the normalized f*/
            t = Point3::VtP(k);
            return true;
        }

        else {
            k = r.getOrigin() + t0 * f;//r.getDirection(); /*or the normalized f*/
            t = Point3::VtP(k);
            return true;
        }
    }
}


double LeafCloud::getName() const
{
    return name;
}

int LeafCloud::size()
{
    return cloudSize;
}

double LeafCloud::influence() {
//     if(indexVal>=0)
//     {
//         int idx = indexVal;
//         indexVal = -1;
//         return leaves[idx]->influence();
//     }
     if(leaves.size() > 0)
     {
         return leaves[0]->influence();
     }
}

void LeafCloud::lifeExtend()
{
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->lifeExtend();
    }
}

bool LeafCloud::lifeDegrade()
{
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->lifeDegrade();
    }
}

void LeafCloud::setCloudId(int val)
{
    cloudID = val;
}

void LeafCloud::changeParams(Vector4 *sizeChanges, Vector4 *paramChanges, int shapeChange)
{
    //SizeChanges: rx, ry, rz, cloudsize
    //ParamChanges: numOfNodes, kill, influ, seed
    //ShapeChange: Canopy change

//    if(leavesLeft != cloudSize)
//        return;

    if(((*sizeChanges) != cloudRads || shapeChange != cloudType) && leavesLeft == cloudSize)
    {
        for(int i = 0; i < leaves.size(); ++i)
        {
            delete leaves[i];
        }

        leaves.clear();

        cloudType = shapeChange;
        cloudRads = *sizeChanges;
        cloudSize = paramChanges->x;

        createCloud(Vector3(paramChanges->y, paramChanges->z, paramChanges->w));        
        updateAABB(&center);
    }
    else
    {
        if(paramChanges->x != cloudSize && leavesLeft == cloudSize)
        {
            int diff = paramChanges->x - cloudSize;
            if(diff < 0)
            {
                for(int i = cloudSize-1; i > cloudSize + diff - 1; --i)
                {
                    delete leaves[i];
                    leaves.erase(leaves.begin()+i);
                }
            }
            else
            {
                Point3 pt(0, 0, 0);
                //srand((int)paramChanges->w);

                for (int i = cloudSize; i < cloudSize + diff; ++i) {
                    while (true) {
                        pt.y = (cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;
                        pt.x = (cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;
                        pt.z = (cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;

                        //based on the users choice different point clouds are made.
                        if(cloudType == 0) { // Sphere
                            double len = sqrt(pt.x*pt.x + pt.y*pt.y + pt.z*pt.z);
                            if (len > 0 && len < cloudRads.w/3.0) {
                                break;
                            }
                        }
                        else if(cloudType == 1 || cloudType == 6) // Cone
                        {
                            double r, h, h1, r1;
                            r = cloudRads.x;//cS*xx;
                            h = 2*cloudRads.y;//cS/yy;
                            h1 = cloudRads.y;//h/2;

                           if((pt.y>-h1)&&(pt.y<h1))
                            {
                                r1 = (r*(/*-h + pt.y*/pt.y + h1))/h;
                                if((pow(pt.x,2)+pow(pt.z,2)) <= pow(r1,2)) //pt.x < r1 && pt.z < r1)
                                {
                                    pt.y = cloudType == 1 ? -pt.y : pt.y;
                                    //pt.y = pt.y < 0 ? pt.y : -pt.y;
                                    break;
                                }
                            }
                        }
                        else if(cloudType == 2) { // Ellipse
                            double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                            if(test<=1) {
                                  break;
                            }
                        }
                        else if(cloudType == 3) { // Cube
                            if(((pt.x<=cloudRads.x&&pt.x>=-cloudRads.x)&&(pt.y<=cloudRads.y&&pt.y>=-cloudRads.y))&&(pt.z<=cloudRads.z&&pt.z>=-cloudRads.z)) {
                                break;
                            }
                        }
                        else if(cloudType == 4)
                        {
                            double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                            if(test<=1 && pt.y >= getCenter().y) {
                                  break;
                            }
                        }
                        else if(cloudType == 5) // hemi-ellipse
                        {
                            double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                            if(test<=1 && pt.y <= getCenter().y) {
                                  break;
                            }
                        }
                    }

                    leaves.push_back(new Leaf(pt, paramChanges->y, paramChanges->z, name, getCloudId()));
                }
            }
            cloudSize += diff;
            leavesLeft = leaves.size();
        }
        if(paramChanges->y != leaves[0]->radius() || paramChanges->z != leaves[0]->influence())
        {
            for(int i = 0; i < leaves.size(); ++i)
            {
                leaves[i]->radius(paramChanges->y);
                leaves[i]->influence(paramChanges->z);
            }
        }
    }
}

 double LeafCloud::radius() {
//     if(indexVal>=0)
//     {
//         int idx = indexVal;
//         indexVal = -1;
//         return leaves[idx]->radius();
//     }
     if(leaves.size() > 0)
     {
         return leaves[0]->radius();
     }
 }

double LeafCloud::cloudRad() {
     return cloudRads.w;
 }

bool LeafCloud::influence(Point3* node) {
    if(indexVal>=0)
    {
        int idx = indexVal;

        double test = location().distanceTo((*node));
        if(test <= leaves[idx]->influence())
            return true;
        else// if(test > influ)
            return false;
     }
    // May want to do error handeling here
    else
        return false;
}

void LeafCloud::draw(bool shell, bool solid, bool leaf, bool kr, bool ir)
{
    if((!shell && leaf) || (shell && !solid && leaf))
    {
        for(int i = 0; i < size(); ++i)
        {
            indexVal = i;
            leaves[i]->draw(location(), leaf, kr, ir, selection());
        }
    }


    if(shell)
    {
        if(solid)
            glColor3f(getName(), getName(), getName());
        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(getName(), getName(), getName(), 0.3);
        }
        switch(cloudType)
        {
        case 0:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y, getCenter().z);
            gluSphere(quadricSph, cloudRads.x, 30, 30);
            glPopMatrix();
            //return;
            break;
        case 1:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y - cloudRads.y, getCenter().z);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(quadricEli, cloudRads.x, 0, cloudRads.y*2, 30, 30);
            glPopMatrix();
            //return;
            break;
        case 2:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y, getCenter().z);
            glScalef(cloudRads.x, cloudRads.y, cloudRads.z);
            gluSphere(quadricSph, 1, 30, 30);
            glPopMatrix();
            //return;
            break;
        case 3:
            glPushMatrix();
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
            glPopMatrix();
            //return;
            break;
        case 4:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y, getCenter().z);
            glScalef(cloudRads.x, cloudRads.y, cloudRads.z);
            gluSphere(quadricSph, 1, 30, 30);
            glRotated(90, 1, 0, 0);
            gluDisk(quadricShe, 0, 1, 15, 15);
            glPopMatrix();
            //return;
            break;
        case 5:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y, getCenter().z);
            glScalef(cloudRads.x, cloudRads.y, cloudRads.z);
            gluSphere(quadricSph, 1, 30, 30);
            glRotated(-90, 1, 0, 0);
            gluDisk(quadricShe, 0, 1, 15, 15);
            glPopMatrix();
            //return;
            break;
        case 6:
            glPushMatrix();
            glTranslatef(getCenter().x, getCenter().y + cloudRads.y, getCenter().z);
            glRotatef(90, 1, 0, 0);
            gluCylinder(quadricEli, cloudRads.x, 0, cloudRads.y*2, 30, 30);
            glPopMatrix();
            //return;
            break;
        }
        glDisable(GL_BLEND);
    }
}

void LeafCloud::updateAABB(Point3 *newPos)
{
    positions[0] = Point3(newPos->x - cloudRads.x, newPos->y - cloudRads.y, newPos->z - cloudRads.z);
    positions[1] = Point3(positions[0].x, positions[0].y, positions[0].z + cloudRads.z*2);
    positions[2] = Point3(positions[0].x, positions[0].y + cloudRads.y*2, positions[0].z + cloudRads.z*2);
    positions[3] = Point3(positions[0].x, positions[0].y + cloudRads.y*2, positions[0].z);
    positions[4] = Point3(positions[0].x + cloudRads.x*2, positions[0].y, positions[0].z);
    positions[5] = Point3(positions[0].x + cloudRads.x*2, positions[0].y, positions[0].z + cloudRads.z*2);
    positions[6] = Point3(positions[0].x + cloudRads.x*2, positions[0].y + cloudRads.y*2, positions[0].z + cloudRads.z*2);
    positions[7] = Point3(positions[0].x + cloudRads.x*2, positions[0].y + cloudRads.y*2, positions[0].z);
}

void LeafCloud::Leaf::draw(Point3 loca, bool self, bool kill, bool infl, bool sel)
{
    GLUquadricObj *quadricSphSub = gluNewQuadric();
    if(active())
    {
        if(self)
        {
            glPushMatrix();
            if(!sel)
                glColor3f(name, name, name);
            else
                glColor3f (1.0f, 0.2f, 0.2f);

            glTranslatef(loca.x, loca.y, loca.z);
            gluSphere(quadricSphSub, 0.65, 15, 15);
            glPopMatrix();
        }

        if(kill)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();

            if(!sel)
                glColor4f(name+0.3, name+0.1, name+0.5, 0.3);
            else
                glColor3f (1.0f, 0.2f, 0.2f);

            glTranslatef(loca.x, loca.y, loca.z);
            gluSphere(quadricSphSub, rad, 15, 15);
            glPopMatrix();
            glDisable(GL_BLEND);
        }

        if(infl)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();

            if(!sel)
                glColor4f(name+0.1, name+0.5, name+0.3, 0.3);
            else
                glColor3f (1.0f, 0.2f, 0.2f);

            glTranslatef(loca.x, loca.y, loca.z);
            gluSphere(quadricSphSub, influ, 15, 15);
            glPopMatrix();
            glDisable(GL_BLEND);
        }
    }
    else
        return;
}

const int LeafCloud::remaining()
{
    return leavesLeft;
}

const Point3 LeafCloud::location(){
     if(indexVal>=0)
     {
         int idx = indexVal;

         return (leaves[idx]->location() + getCenter());
     }
}

const Point3& LeafCloud::getCenter(){
    return center;
}

void LeafCloud::changeCenter(Point3 *change){
    center = *change;
    updateAABB(change);
}

bool LeafCloud::active() {
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->active();
    }
}

bool LeafCloud::inme(Point3* node) {
    if(indexVal>=0)
    {
        int idx = indexVal;
        double test = location().distanceTo((*node));
        if(test <= leaves[idx]->radius())
            return true;
        else if(test > leaves[idx]->radius())
            return false;
    }
}

int LeafCloud::checkBranch(Branch* tim)
{
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->checkBranch(tim);
    }
}

void LeafCloud::influence(double set)
{
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->influence(set);
    }
}

void LeafCloud::radius(double set)
{
    if(indexVal>=0)
    {
        int idx = indexVal;
        indexVal = -1;
        return leaves[idx]->radius(set);
    }
}

bool LeafCloud::addLeaf(Point3 locale, double r, double in, int val)
{
    leaves.push_back(new Leaf(locale, r, in, val, getCloudId()));
    return true;
}

void LeafCloud::kill() {
     if(indexVal>=0)
     {
         int idx = indexVal;
         indexVal = -1;
         leavesLeft--;
         return leaves[idx]->kill();
     }
}

double UD ( int seed )
{
   return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

void LeafCloud::createCloud(Vector3 params)
{
    Point3 pt(0, 0, 0);
    srand((int)params.z);
    int rm = 0;

    for (int i = 0; i < cloudSize; i++) {
        while (true) {
            pt.y = -IM + UD( rand() ) * ( 2 * IM );//(cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;
            pt.x = -IM + UD( rand() ) * ( 2 * IM );//(cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;
            pt.z = -IM + UD( rand() ) * ( 2 * IM );//(cloudRads.w*rand())/RAND_MAX - cloudRads.w/2.0;

            //based on the users choice different point clouds are made.
            if(cloudType == 0) { // Sphere
                double len = sqrt(pt.x*pt.x + pt.y*pt.y + pt.z*pt.z);
                if (len > 0 && len < cloudRads.x) {// cloudRads.w/3.0) {
                    break;
                }
            }
            else if(cloudType == 1 || cloudType == 6) // Cone
            {
                double r, h, h1, r1;
                r = cloudRads.x;//cS*xx;
                h = 2*cloudRads.y;//cS/yy;
                h1 = cloudRads.y;//h/2;

               if((pt.y>-h1)&&(pt.y<h1))
                {
                    r1 = (r*(/*-h + pt.y*/pt.y + h1))/h;
                    if((pow(pt.x,2)+pow(pt.z,2)) <= pow(r1,2)) //pt.x < r1 && pt.z < r1)
                    {
                        pt.y = cloudType == 1 ? -pt.y : pt.y;
                        //pt.y = pt.y < 0 ? pt.y : -pt.y;
                        break;
                    }
                }
            }
            else if(cloudType == 2) { // Ellipse
                double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                if(test<=1) {
                      break;
                }
            }
            else if(cloudType == 3) { // Cube
                if(((pt.x<=cloudRads.x&&pt.x>=-cloudRads.x)&&(pt.y<=cloudRads.y&&pt.y>=-cloudRads.y))&&(pt.z<=cloudRads.z&&pt.z>=-cloudRads.z)) {
                    break;
                }
            }
            else if(cloudType == 4)
            {
                double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                if(test<=1 && pt.y >= getCenter().y) {
                      break;
                }
            }
            else if(cloudType == 5) // hemi-ellipse
            {
                double test = ((pow(pt.x-0, 2))/(cloudRads.x*cloudRads.x)+(pow(pt.y-0, 2))/(cloudRads.y*cloudRads.y)+(pow(pt.z-0, 2))/(cloudRads.z*cloudRads.z));
                if(test<=1 && pt.y <= getCenter().y) {
                      break;
                }
            }
        }

        leaves.push_back(new Leaf(pt, params.x, params.y, name, getCloudId()));
    }
}

const int LeafCloud::getCloudType() const
{
    return cloudType;
}

const int LeafCloud::getCloudId()
{
    return cloudID;
}

void LeafCloud::selection(bool val)
{
    isSelected = val;
}

const bool LeafCloud::selection()
{
    return isSelected;
}
