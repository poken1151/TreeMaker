#include "camera.h"
#include "math.h"

#define SQR(x) (x*x)

CCamera::CCamera()
{
    //Init with standard OGL values:
    Position    = Vector3 (0.0, 0.0, 250.0); //y = 15
    ViewDir     = Vector3 (0.0, 0.3, -1.0);
    ViewPoint   = Vector3 (0, 0, 0);
    RightVector = Vector3 (1.0, 0.0, 0.0);
    UpVector    = Vector3 (0.0, 1.0, 0.0);

    //Only to be sure:
    RotatedX = RotatedY = RotatedZ = 0.0;
}

CCamera::CCamera(Vector3 pos, Vector3 view, Vector3 right, Vector3 up)
{
    //Init with standard OGL values:
    Position    = pos;
    ViewPoint   = view;
    RightVector = right;
    UpVector    = up;
    ViewDir     = ViewPoint - Position;

    //Only to be sure:
    RotatedX = RotatedY = RotatedZ = 0.0;
}

void CCamera::Move (Vector3 dir)
{
    Position = Position + dir;
    ViewPoint = ViewPoint + dir;
}

Vector3 CCamera::getViewPoint ()
{
    //return Vector3(Position.x+ViewDir.x, Position.y+ViewDir.y, Position.z+ViewDir.z);
    return ViewPoint;
}

//void CCamera::setPos (Vector3 np)
//{
void CCamera::setPos (QQuaternion q)
{

//    Vector3 up(0, 1, 0);
//    Position = np;
//    ViewDir = (ViewPoint - Position); ViewDir.normalize();
//    RightVector =  ViewDir ^ up; RightVector.normalize();
//    UpVector = RightVector ^ ViewDir; UpVector.normalize();
////    ViewDir = (ViewPoint - Position); ViewDir.normalize();
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

    QVector3D t = q.rotatedVector(QVector3D(Position.x, Position.y, Position.z));
    Position = Vector3(t.x(), t.y(), t.z());
    ViewDir = (ViewPoint - Position); ViewDir.normalize();
    t = q.rotatedVector(QVector3D(0, 1, 0));
    UpVector = Vector3(t.x(), t.y(), t.z()); UpVector.normalize();
    RightVector = ViewDir ^ UpVector; RightVector.normalize();
    UpVector = RightVector ^ ViewDir; UpVector.normalize();

    qDebug() << "UpVec: " << UpVector.x << " " << UpVector.y << " " << UpVector.z << endl;
    qDebug() << "RightVector: " << RightVector.x << " " << RightVector.y << " " << RightVector.z << endl;
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

void CCamera::RotateX (float ang)
{
    RotatedX += ang;

    //Rotate viewdir around the right vector:
    ViewDir = ViewDir*cos(ang*PIdiv180) + UpVector*sin(ang*PIdiv180);
    ViewDir.normalize();

    //now compute the new UpVector (by cross product)
    UpVector = (ViewDir ^ RightVector)*-1;


}

void CCamera::RotateY (float ang)
{
    RotatedY += ang;

    //Rotate viewdir around the up vector:
    ViewDir = ViewDir*cos(ang*PIdiv180) - RightVector*sin(ang*PIdiv180);
    ViewDir.normalize();

    //now compute the new RightVector (by cross product)
    RightVector = (ViewDir ^ UpVector);
}

void CCamera::RotateZ (float ang)
{
    RotatedZ += ang;

    //Rotate viewdir around the right vector:
    RightVector = RightVector*cos(ang*PIdiv180) + UpVector*sin(ang*PIdiv180);
    RightVector.normalize();

    //now compute the new UpVector (by cross product)
    UpVector = (ViewDir ^ RightVector)*-1;
}

void CCamera::Render( void )
{
    //GLfloat p = -1.0;

    //The point at which the camera looks:
    //Vector3 ViewPoint = Position+ViewDir; //Position+
    //Vector3 ViewPoint(0, 0, 0); //Position+

    //as we know the up vector, we can easily use gluLookAt:
    gluLookAt(	Position.x,Position.y,Position.z,
                ViewPoint.x,ViewPoint.y,ViewPoint.z,
                UpVector.x,UpVector.y,UpVector.z);


}

void CCamera::MoveForward(float Distance )
{
    ViewDir = (ViewPoint - Position); ViewDir.normalize();
    Position = Position + (ViewDir*-Distance);
    //ViewPoint = ViewPoint + (ViewDir*-Distance);
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
