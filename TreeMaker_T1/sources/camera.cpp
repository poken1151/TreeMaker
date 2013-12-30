/*
 * camera.cpp
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
 *     Implemetation file.
 */
#include "camera.h"
#include "math.h"

#define SQR(x) (x*x)

CCamera::CCamera()
{
    Position    = Vector3 (0.0, 0.0, 250.0);
    RightVector = Vector3 (1.0, 0.0, 0.0);
    UpVector    = Vector3 (0.0, 1.0, 0.0);
    PositionOriginal    = Position;
    RightVectorOriginal = RightVector;
    UpVectorOriginal    = UpVector;
    ViewDir     = Vector3 (0.0, 0.3, -1.0);
    ViewPoint   = Vector3 (0, 0, 0);
    tot = QQuaternion();
    totR = QQuaternion();

    RotatedX = 0.0;
    RotatedY = 0.0;
    RotatedZ = 0.0;
}

CCamera::CCamera(Vector3 pos, Vector3 view, Vector3 right, Vector3 up)
{
    Position    = pos;
    RightVector = right;
    UpVector    = up;
    PositionOriginal    = Position;
    RightVectorOriginal = RightVector;
    UpVectorOriginal    = UpVector;
    ViewPoint   = view;
    ViewDir     = ViewPoint - Position;
    tot = QQuaternion();
    totR = QQuaternion();

    RotatedX = 0.0;
    RotatedY = 0.0;
    RotatedZ = 0.0;
}

void CCamera::Move (Vector3 dir)
{
    Position = Position + dir;
    ViewPoint = ViewPoint + dir;
}

Vector3 CCamera::getViewPoint ()
{
    return ViewPoint;
}

void CCamera::setRot(QQuaternion q)
{
//    Vector3 OldViewDir = (Vector3(Position.x, 0, Position.z) - Vector3(ViewPoint.x, 0, ViewPoint.z)); OldViewDir.normalize();
//    double oldDP = Vector3(0, 1, 0) * OldViewDir;

//    tot = tot * q;
//    QVector3D t = tot.rotatedVector(QVector3D(PositionOriginal.x, PositionOriginal.y, PositionOriginal.z));
//    Position = Vector3(t.x(), t.y(), t.z());

//    Vector3 NewViewDir = (Vector3(Position.x, 0, Position.z) - Vector3(ViewPoint.x, 0, ViewPoint.z)); NewViewDir.normalize();
//    double newDP = Vector3(0, 1, 0) * NewViewDir;

//    Vector3 axis = NewViewDir ^ OldViewDir;

////    double angle = acos((NewViewDir * OldViewDir));
//    double angle = 180 / PI * acos((NewViewDir * OldViewDir));

//    if((newDP - oldDP) < 0)
//        angle = -angle;
////    double angle = 180 / PI * acos(sqrt(NewViewDir * OldViewDir));
////    float angle = 180 / PI * asin(sqrt(QVector3D::dotProduct(QVector3D(axis.x, axis.y, axis.z),
////                                                             QVector3D(axis.x, axis.y, axis.z))));

//    QQuaternion nq = QQuaternion::fromAxisAndAngle(0,1,0, angle);
//    totR = totR * nq;

//    ViewDir = (ViewPoint-Position); ViewDir.normalize();
//    t = nq.rotatedVector(QVector3D(RightVector.x, RightVector.y, RightVector.z));
//    //t = totR.rotatedVector(QVector3D(RightVectorOriginal.x, RightVectorOriginal.y, RightVectorOriginal.z));
//    //t = tot.rotatedVector(QVector3D(1, 0, 0));
//    RightVector = Vector3(t.x(), t.y(), t.z()); RightVector.normalize();

//    UpVector = RightVector ^ ViewDir;  UpVector.normalize();


    tot = tot * q;
    QVector3D t = tot.rotatedVector(QVector3D(PositionOriginal.x, PositionOriginal.y, PositionOriginal.z));
    Position = Vector3(t.x(), t.y(), t.z());
    //t = tot.rotatedVector(QVector3D(UpVectorOriginal.x, UpVectorOriginal.y, UpVectorOriginal.z));
    //t = tot.rotatedVector(QVector3D(0, 1, 0));
    //UpVector = Vector3(t.x(), t.y(), t.z()); UpVector.normalize();
    t = tot.rotatedVector(QVector3D(RightVectorOriginal.x, RightVectorOriginal.y, RightVectorOriginal.z));
    //t = tot.rotatedVector(QVector3D(1, 0, 0));
    RightVector = Vector3(t.x(), 0, t.z()); RightVector.normalize();

    ViewDir = (ViewPoint-Position); ViewDir.normalize();

    UpVector = RightVector ^ ViewDir;  UpVector.normalize();

//        qDebug() << "Dot Product: " << dpro << "\n";
//        qDebug() << "ViewDir: " << ViewDir.x << " " << ViewDir.y << " " << ViewDir.z << "\n";
//        qDebug() << "UpVec: " << UpVector.x << " " << UpVector.y << " " << UpVector.z << "\n";
//        qDebug() << "RightVector: " << RightVector.x << " " << RightVector.y << " " << RightVector.z << endl;
//        qDebug() << "Axis: " << axis.x << ", " << axis.y <<", " << axis.z << "\n";
//        qDebug() << "Angle: " << angle << "\n";
//        qDebug() << "NewViewDir: " << NewViewDir.x << " " << NewViewDir.y << " " << NewViewDir.z << "\n";
//        qDebug() << "OldViewDir: " << OldViewDir.x << " " << OldViewDir.y << " " << OldViewDir.z << "\n";
}
void CCamera::setPos (Vector3 np)
{
//void CCamera::setPos (QQuaternion q)
//{

    Vector3 up(0, 1, 0);

    Position = np;
    ViewDir = (ViewPoint - Position); ViewDir.normalize();
    double dpro = up * UpVector;

    RightVector =  ViewDir ^ up; RightVector.normalize();
    UpVector = RightVector ^ ViewDir;  UpVector.normalize();


//    ViewDir = (ViewPoint - Position); ViewDir.normalize();
//    double dproUP = ViewDir * UpVector;

//    Position = np;
//    ViewDir = (ViewPoint - Position); ViewDir.normalize();
//    double dproUP2 = ViewDir * UpVector;

//    if(dproUP == dproUP2)
//    {
//        RightVector = (ViewDir ^ UpVector); RightVector.normalize();
//    }
//    else
//    {
//        UpVector = (ViewDir ^ RightVector)*-1; UpVector.normalize();
//        RightVector = (ViewDir ^ UpVector); RightVector.normalize();
//    }

    //UpVector = (ViewDir ^ RightVector)*-1;
//    ViewDir = (ViewPoint - Position); ViewDir.normalize();

//    QVector3D t = q.rotatedVector(QVector3D(Position.x, Position.y, Position.z));
//    Position = Vector3(t.x(), t.y(), t.z());
//    t = q.rotatedVector(QVector3D(UpVector.x, UpVector.y, UpVector.z));
//    UpVector = Vector3(t.x(), t.y(), t.z());
//    t = q.rotatedVector(QVector3D(RightVector.x, RightVector.y, RightVector.z));
//    RightVector = Vector3(t.x(), t.y(), t.z());

//    qDebug() << "Dot Product: " << dpro << "\n";
//    qDebug() << "ViewDir: " << ViewDir.x << " " << ViewDir.y << " " << ViewDir.z << "\n";
//    qDebug() << "UpVec: " << UpVector.x << " " << UpVector.y << " " << UpVector.z << "\n";
//    qDebug() << "RightVector: " << RightVector.x << " " << RightVector.y << " " << RightVector.z << endl;
}


Vector3 CCamera::getPos ()
{
    return Position;
}

Vector3 CCamera::getViewDir ()
{
    return ViewDir;
}

Vector3 CCamera::getUp ()
{
    return UpVector;
}

void CCamera::Render( void )
{
    gluLookAt(	Position.x,Position.y,Position.z,
                ViewPoint.x,ViewPoint.y,ViewPoint.z,
                UpVector.x,UpVector.y,UpVector.z);


}

void CCamera::MoveForward(float Distance )
{
    ViewDir = (ViewPoint - Position); ViewDir.normalize();
    Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight (float Distance )
{
    Position = Position + (RightVector*Distance);
    ViewPoint = ViewPoint + (RightVector*Distance);
}

void CCamera::MoveUpward(float Distance )
{
    Position = Position + (UpVector*Distance);
    ViewPoint = ViewPoint + (UpVector*Distance);
}
