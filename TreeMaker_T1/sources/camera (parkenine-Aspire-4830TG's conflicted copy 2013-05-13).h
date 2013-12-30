//***************************************************************************
//
// Advanced CodeColony Camera
// Philipp Crocoll, 2003
// This is a publicly available implementation and I have used it with such permision from the creater
// for this project for this course. It is slightly modified by me, but from the above reference
//
//***************************************************************************
#ifndef CAMERA_H
#define CAMERA_H

//#include <GL/glu.h>		// Need to include it here because the GL* types are required
//#include <GL/GLU.h>

#include "vecmath.h"
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

struct SF3dVector  //Float 3d-vect, normally used
{
    GLfloat x,y,z;
};
struct SF2dVector
{
    GLfloat x,y;
};
SF3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z );

class CCamera
{
private:

    Vector3 ViewDir;
    Vector3 ViewPoint;
    Vector3 RightVector;
    Vector3 UpVector;
    Vector3 Position;

    float RotatedX, RotatedY, RotatedZ;

public:
    CCamera();
    CCamera(Vector3 pos, Vector3 view, Vector3 right, Vector3 up);
    void Render ( void );

    void Move ( Vector3 dir );
    void RotateX ( float ang );
    void RotateY ( float ang );
    void RotateZ ( float ang );

    Vector3 getUp ();
    Vector3 getPos ();
    Vector3 getViewDir ();
    Vector3 getViewPoint ();

    void MoveForward ( float Distance );
    void MoveUpward ( float Distance );
    void StrafeRight ( float Distance );


    //void setPos(Vector3 np);
    void setPos(QQuaternion q);
};
#endif

