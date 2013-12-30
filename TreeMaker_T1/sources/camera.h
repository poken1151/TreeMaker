/*
 * camera.h
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
 *     A basic camera class to navigate a scene. Holds a Right, Up and View
 *     vector, along with a focous point. It holds a quaternion to maintain
 *     total rotation and apply to vectors.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "vecmath.h"
#define PI 3.1415926535897932384626433832795

class CCamera
{
private:

    Vector3 ViewDir;
    Vector3 ViewPoint;
    Vector3 RightVector;
    Vector3 UpVector;
    Vector3 Position;
    Vector3 RightVectorOriginal;
    Vector3 UpVectorOriginal;
    Vector3 PositionOriginal;
    Vector4 quat;
    QQuaternion tot;
    QQuaternion totR;

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


    void setPos(Vector3 np);
    void setRot(QQuaternion q);
};
#endif

