/*
 * glwidget.h
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
#include <QtWidgets>
#include <QtDebug>
#include <math.h>
#include <stack>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromRgb(235, 235, 255);

    quadricCyl = gluNewQuadric();

    boxPos = new Point3(0, 0, 0);
    cloudPos = new Point3(0, 0, 0);
    trunkStartPos = new Point3(0, 0, 0);
    boxSize = new Vector4(0, 0, 0, 0);
    boxDimensions = new Vector4(0, 0, 0, 0);
    cloudDimensions = new Vector4(0, 0, 0, 0);
    cloudParams = new Vector4(0, 0, 0, 0);
    int cloudShapetoEdit = 2;
    currentTreeGirth = 1.0;
    currentGrowLimit = 5;

    drawShell = false;
    drawTrans = true;
    drawSolid = false;
    drawNodes = true;
    drawKillR = false;
    drawImflR = false;
    drawBoxes = true;
    drawBoxWi = false;
    drawTreeF = false;
    growOption = true;
    forceUpdateScene = false;
    drawABbox = false;
    gloPos = Vector3(0, 0, 100);

    for(int i = 0; i < 3; ++i)
    {
        gizmo.push_back(new AABB(new Point3(0, 0, 0), i));
    }
    currentGizmoIndex = 0;
    currentGizmoType = 0;

    sizeW = 0;
    sizeH = 0;

    nodeToBranchOpt = false;
    nodeToCenterOpt = false;
    nodeInBlockOpt = false;
    axisCamera = CCamera(Vector3 (0.0, 0.0, 10.0), Camera.getViewPoint(), Vector3 (1.0, 0.0, 0.0), Camera.getUp());
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(960, 600);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(sizeW, sizeH);
}
//! [4]

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


//! [6]
void GLWidget::initializeGL()
{
    qglClearColor(qtPurple);

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL) ;

    static GLfloat lightPosition1[4] = { 0, 0, 0, 1.0 };//{ -100.0, 200, -100, 1.0 };
    static GLfloat lightPosition2[4] = { 100.0, 200, -100, 1.0 };
    static GLfloat lightPosition3[4] = { 100.0, 200, 100, 1.0 };
    static GLfloat lightPosition4[4] = { -100.0, 200, 100, 1.0 };
//    float specular[] = {1.0f, 1.0f, 1.0f , 1.0f};
//    float ambient[] = {0.2f, 0.2f, 0.2f};
//    float diffuse[] = {0.8f, 0.8f, 0.8f , 0.8f};
//    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
//    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
//    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
//    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
//    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition4);

    glEnable(GL_LIGHTING);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHT2);
//    glEnable(GL_LIGHT3);

    float global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    currentSelectedIndex = NULL;
    currentSelectedType= NULL;

    Point3 aa = Point3(-10000, -10000, -10000);
    Point3 bb = Point3(10000, 10000, 10000);
    lineEnd = Point3(0, 0, 0);
    lineStart = Point3(-0.01, 34, 199);

    lastSize = 0;

    Octree = Octnode(aa, bb, 200);

    maxAgeLimit = 0;
}
//! [6]

void GLWidget::drawLiners()
{
//    glPushMatrix();
//    glBegin(GL_LINES);
//        glColor3f(0.055f, 0.055f, 0.055f);
//        glEnable( GL_LINE_SMOOTH );
//        glLineWidth( 1.5 );
//        glVertex3f(lineStart.x, lineStart.y, lineStart.z);
//        glVertex3f(lineEnd.x, lineEnd.y, lineEnd.z);
//    glEnd();
//    glPopMatrix();
}

//! [7]
void GLWidget::paintGL()
{

//    GLUquadricObj* cyl;
//    cyl = gluNewQuadric();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    draw(false);
}
//! [7]

void GLWidget::draw(bool solid)
{
    drawAxis();

    glLoadIdentity();

    Camera.Render();

    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.0);

        glBegin(GL_QUADS);
            glColor3f(0.75f, 0.75f, 0.75f);
            glVertex3f(50.0f, 0.0f, 50.0f);
            glVertex3f(50.0f, 0.0f, -50.0f);
            glVertex3f(-50.0f, 0.0f, -50.0f);
            glVertex3f(-50.0f, 0.0f, 50.0f);
        glEnd();
    glPopMatrix();

    for(uint i = 0; i < AllObstructions.size(); ++i)
    {
        AllObstructions[i]->draw(drawBoxes, drawBoxWi);
    }

    for(uint i = 0; i < AllLeafClouds.size(); ++i)
    {
        AllLeafClouds[i]->draw(drawShell, drawSolid, drawNodes, drawKillR, drawImflR);
    }

    if(currentSelectedType != 0)
        drawGizmo();

    if(drawABbox)
    {
        drawAABB(&Octree);
    }

    glEnable(GL_LIGHTING);
        drawBranches(false);
    glDisable(GL_LIGHTING);


//    glPushMatrix();
//    glColor3f(1.0, 0, 1.0);
//    glTranslatef(gloPos.x, gloPos.y, gloPos.z);
//    gluSphere(quadricCyl, 5, 30, 30);
//    glPopMatrix();
}

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    float ratio = (1.0* width)/(1.0* height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    gluPerspective(45.0f, ratio,0.1f, 1500.0f);
    glMatrixMode(GL_MODELVIEW);

    sizeW = width;
    sizeH = height;
}
//! [8]

    void GLWidget::drawAxis()
    {
//          glPushMatrix();
//          glLineWidth( 5.0f );
//          glBegin(GL_LINES);
//              glEnable(GL_LINE_SMOOTH);

//              glColor3f(0.0f, 0.0f, 1.0f);
//              glVertex3f(-50, 0, -50);
//              glVertex3f(-50, 0, 50);

//              glColor3f(0.0f, 1.0f, 0.0f);
//              glVertex3f(-50, 0, -50);
//              glVertex3f(-50, 100, -50);
////              glColor3f(0.0f, 1.0f, 0.0f);
////              glVertex3f(0, 0, 0);
////              glVertex3f(0, 100, 0);

//              glColor3f(1.0f, 0.0f, 0.0f);
//              glVertex3f(-50, 0, -50);
//              glVertex3f(50, 0, -50);
//          glEnd();
//          glPopMatrix();

                glViewport(0, 0, 50, 50);
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluPerspective(45.0f, 1.0f, 0.1f, 20.0f);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                glDisable(GL_CULL_FACE);

                glClear(GL_DEPTH_BUFFER_BIT);
                //gluLookAt(0.0f, 0.0f, -10.0f, 0.0f, 0.0f, 10.0f, 0.0f,0.1f,0.0f);
                axisCamera.Render();

                glColor3f(1.0f, 0.0f, 0.0f);
                glEnable( GL_LINE_SMOOTH );
                glLineWidth( 1.5 );
                glPushMatrix();
                glBegin(GL_LINES);
                    glVertex3f(0, 0, 0);
                    glVertex3f(4, 0, 0);
                glEnd();
                glPopMatrix();

                glColor3f(0.0f, 1.0f, 0.0f);
                glEnable( GL_LINE_SMOOTH );
                glLineWidth( 1.5 );
                glPushMatrix();
                glBegin(GL_LINES);
                    glVertex3f(0, 0, 0);
                    glVertex3f(0, 4, 0);
                glEnd();
                glPopMatrix();

                glColor3f(0.0f, 0.0f, 1.0f);
                glEnable( GL_LINE_SMOOTH );
                glLineWidth( 1.5 );
                glPushMatrix();
                glBegin(GL_LINES);
                    glVertex3f(0, 0, 0);
                    glVertex3f(0, 0, 4);
                glEnd();
                glPopMatrix();

                // Making sure we can render 3d again

                glViewport(0, 0, sizeHint().width(), sizeHint().height());
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                gluPerspective(45.0f, (sizeHint().width()/sizeHint().height()), 0.1f, 400.0f);

//        double modelViewMatrix[16];
//        double projectionMatrix[16];
//        int viewport[4];

//        glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
//        glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
//        glGetIntegerv(GL_VIEWPORT, viewport);

//        glLoadIdentity();
//        glMatrixMode(GL_PROJECTION);

//        glViewport(0, 0, 50, 50);

//        gluPerspective(45.0f, 1,0.1f,400.0f);

//        glMatrixMode(GL_PROJECTION);
//        glViewport(0, 0, 960, 600);
//        glLoadMatrixf(temp);
    }

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::MiddleButton || event->buttons() & Qt::RightButton)
    {}
    else
        objectAtPosition(event->pos());

    lastPos = event->pos();
}
//! [9]

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    currentGizmoType = 0;
}

Vector3 GLWidget::arcVec(const QPoint &pos) {
  Vector3 P = Vector3(1.0*pos.x() / sizeHint().width() * 2 - 1.0,
                      1.0*pos.y() / sizeHint().height() * 2 - 1.0,
                      0.0);
  P.x = -P.x;
  float OP_squared = P.x * P.x + P.y * P.y;
  if (OP_squared <= 1*1)
    P.z = sqrt(1*1 - OP_squared);
  else
    P.normalize();
  return P;

//  Vector3 P = Vector3(pos.x(), pos.y(), 0.0f);
//  float fz = 1 - (P * P);
//  if (fz > 0)
//      P.z = sqrt(fz);
//  else
//      P.normalize();
//  return P;
}

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    double dx = event->x() - lastPos.x();
    double dy = event->y() - lastPos.y();

    if ((event->buttons() & Qt::LeftButton) && currentGizmoType == 3 )
    {
        Point3 hitPoint = virtualPlane(event->pos());
        Vector3 changePoint = hitPoint - lineEnd;
        lineEnd = hitPoint;
        Point3 np = gizmo[0]->getPosition();

        switch(currentGizmoIndex)
        {
        case 0:
            np.x += changePoint.x;
            break;
        case 1:
            np.y += changePoint.y;
            break;
        case 2:
            np.z += changePoint.z;
            break;
        }

        for(int i = 0; i < 3; ++i)
        {
            gizmo[i]->changePosition(&np);
        }


        switch(currentSelectedType)
        {
        case 1:
            *cloudPos = np;
            AllLeafClouds[currentSelectedIndex]->changeCenter(cloudPos);
            break;
        case 2:
            *boxPos = np;
            //AllBoxes[currentSelectedIndex]->changePosition(boxPos);
            AllObstructions[currentSelectedIndex]->changePosition(boxPos);
            break;
        }

        updateGL();
    }

    else if (event->buttons() & Qt::RightButton)
    {
        Vector3 P0 = arcVec(lastPos);
        Vector3 P1 = arcVec(event->pos());


        Vector3 axis = P0^P1;// axis.normalize();
//        axis.z = 0;
//        double angle = acos(min(1.0, P0*P1));
//        float angle = 180 / PI * asin(sqrt(QVector3D::dotProduct(QVector3D(axis.x, axis.y, axis.z),
//                                                                 QVector3D(axis.x, axis.y, axis.z))));
//        float angle = 180 / PI * acos(sqrt(P0*P1));
//        float angle = 180 / PI * acos(sqrt(min(1.0, P0*P1)));
//        float angle = 180 / PI * acos(min(1.0, P0*P1));
        float angle = 180 / PI * acos(P0*P1);

        QQuaternion q = QQuaternion::fromAxisAndAngle(axis.x, axis.y, axis.z, angle);
        QVector3D t = q.rotatedVector(QVector3D(Camera.getPos().x, Camera.getPos().y, Camera.getPos().z));
        Camera.setPos(Vector3(t.x(), t.y(), t.z()));
        t = q.rotatedVector(QVector3D(axisCamera.getPos().x, axisCamera.getPos().y, axisCamera.getPos().z));
        axisCamera.setPos(Vector3(t.x(), t.y(), t.z()));
//        Camera.setRot(q);
//        axisCamera.setRot(q);
//        angle = -angle;
//        //axis = -axis;
//        axis.x = 0;
//        axis.y = 0;
//        q = QQuaternion::fromAxisAndAngle(axis.x, axis.y, axis.z, angle);
//        Camera.setRot(q);
//        axisCamera.setRot(q);
//        qDebug() << "Axis: " << axis.x << ", " << axis.y <<", " << axis.z << "\n";
//        qDebug() << "Angle: " << angle << "\n";
//        QVector3D t = q.rotatedVector(QVector3D(gloPos.x, gloPos.y, gloPos.z));
//        gloPos = Vector3(t.x(), t.y(), t.z());
//        Camera.setPos(q);
    }
//    else if (event->buttons() & Qt::RightButton)
//    {
//        Camera.RotateX(dy / 10);
//        Camera.RotateY(dx / 10);
//    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        Camera.MoveUpward(dy / 10);
        Camera.StrafeRight(-dx / 10);
    }
    lastPos = event->pos();
    updateGL();
}
//! [10]

void GLWidget::wheelEvent(QWheelEvent *event)
{
    //QPoint pos = event->angleDelta();
    if(true)//currentSelectedType == 0)
    {
        Camera.MoveForward(-(event->angleDelta().y() / 20));
        updateGL();
    }
    else if(QApplication::keyboardModifiers() == Qt::AltModifier)
    {
        if(QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            if(currentSelectedType == 1)
                setXPos((-(event->delta() / 12) + cloudPos->x) * 100);
            else
                setXPos((-(event->delta() / 12) + boxPos->x) * 100);
        }
        else if(QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            if(currentSelectedType == 1)
                setZPos((-(event->delta() / 12) + cloudPos->z) * 100);
            else
                setZPos((-(event->delta() / 12) + boxPos->z) * 100);
        }
        else
        {
            if(currentSelectedType == 1)
                setYPos(((event->delta() / 12) + cloudPos->y) * 100);
            else
                setYPos(((event->delta() / 12) + boxPos->y) * 100);
        }
    }
    else
    {
        if(QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            if(currentSelectedType == 1)
                setXPos((-(event->delta() / 120) + cloudPos->x) * 100);
            else
                setXPos((-(event->delta() / 120) + boxPos->x) * 100);
        }
        else if(QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            if(currentSelectedType == 1)
                setZPos((-(event->delta() / 120) + cloudPos->z) * 100);
            else
                setZPos((-(event->delta() / 120) + boxPos->z) * 100);
        }
        else
        {
            if(currentSelectedType == 1)
                setYPos(((event->delta() / 120) + cloudPos->y) * 100);
            else
                setYPos(((event->delta() / 120) + boxPos->y) * 100);
        }
    }
}

Point3 GLWidget::virtualPlane(const QPoint &pos)
{
    Point3 camPoint = Point3::VtP(Camera.getPos());
    Vector3 vn = Point3::PtV(camPoint); vn.y = 0.0;
    Vector3 n = Point3::PtV(lineEnd); n.y = 0.0;
    n = vn - n; n.normalize();

    double myY = sizeHint().height() - pos.y();
    double modelViewMatrix[16];
    double projectionMatrix[16];
    int viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    double nearPlaneLocation[3];
    gluUnProject(pos.x(), myY, 0.0, modelViewMatrix, projectionMatrix,
        viewport, &nearPlaneLocation[0], &nearPlaneLocation[1],
        &nearPlaneLocation[2]);

    double farPlaneLocation[3];
    gluUnProject(pos.x(), myY, 1.0, modelViewMatrix, projectionMatrix,
        viewport, &farPlaneLocation[0], &farPlaneLocation[1],
       &farPlaneLocation[2]);

    Ray newRay(Point3(nearPlaneLocation[0], nearPlaneLocation[1], nearPlaneLocation[2]),
            Point3(farPlaneLocation[0], farPlaneLocation[1], farPlaneLocation[2]));

    Vector3 f = newRay.getDirection();
    f.normalize();

    double dpro = f*n;

    if((dpro<0)&&(dpro>-0)) return Point3();
    Point3 temp = Point3::VtP(newRay.getOrigin());

    double L = (n * (lineEnd - temp)) / dpro;

    if(L<-0) return Point3();

    return Point3::VtP(newRay.getOrigin() + L*f);
}

int GLWidget::objectAtPosition(const QPoint &pos)
{
    Point3 tempPoint(0, 0, 0);
    Point3 camPoint = Point3::VtP(Camera.getPos());
    double minDist = DM;
    int idx = -1;
    int tpe = 0;

    double myY = sizeHint().height() - pos.y();
    double modelViewMatrix[16];
    double projectionMatrix[16];
    int viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    double nearPlaneLocation[3];
    gluUnProject(pos.x(), myY, 0.0, modelViewMatrix, projectionMatrix,
        viewport, &nearPlaneLocation[0], &nearPlaneLocation[1],
        &nearPlaneLocation[2]);

    double farPlaneLocation[3];
    gluUnProject(pos.x(), myY, 1.0, modelViewMatrix, projectionMatrix,
        viewport, &farPlaneLocation[0], &farPlaneLocation[1],
       &farPlaneLocation[2]);

    Ray newRay(Point3(nearPlaneLocation[0], nearPlaneLocation[1], nearPlaneLocation[2]),
            Point3(farPlaneLocation[0], farPlaneLocation[1], farPlaneLocation[2]));

    bool hit = false;

    if(currentSelectedType != 0)
        for(uint i = 0; ((i < 3)); ++i)
        {
            hit = gizmo[i]->intersectionTest(newRay, tempPoint);
            if(hit && (camPoint.distanceToSquared(tempPoint) < minDist))
            {
                idx = i;
                tpe = 3;
                minDist = camPoint.distanceToSquared(tempPoint);
                hit = false;
            }
        }

    if(tpe != 3)
    {
        for(uint i = 0; ((i < AllLeafClouds.size())); ++i)
        {
            hit = AllLeafClouds[i]->intersectionTest(newRay, tempPoint);
            if(hit && (camPoint.distanceToSquared(tempPoint) < minDist))
            {
                idx = i;
                tpe = 1;
                minDist = camPoint.distanceToSquared(tempPoint);
                hit = false;
            }
        }
        //qDebug() << "(" << tempPoint.x << ". " << tempPoint.y << ". " << tempPoint.z << ")\n";

//        for(uint i = 0; ((i < AllBoxes.size())); ++i)
//        {
//            hit = AllBoxes[i]->intersectionTest(newRay, tempPoint);
//            if(hit && (camPoint.distanceToSquared(tempPoint) < minDist))
//            {
//                idx = i;
//                tpe = 2;
//                minDist = camPoint.distanceToSquared(tempPoint);
//                hit = false;
//            }
//        }

        for(uint i = 0; ((i < AllObstructions.size())); ++i)
        {
            hit = AllObstructions[i]->intersectionTest(newRay, tempPoint);
            if(hit && (camPoint.distanceToSquared(tempPoint) < minDist))
            {
                idx = i;
                tpe = 2;
                minDist = camPoint.distanceToSquared(tempPoint);
                hit = false;
            }
        }
        //qDebug() << "(" << tempPoint.x << ". " << tempPoint.y << ". " << tempPoint.z << ")\n";

        switch(currentSelectedType)
        {
        case 1:
            AllLeafClouds[currentSelectedIndex]->selection(false);
            break;
        case 2:
            //AllBoxes[currentSelectedIndex]->selection(false);
            AllObstructions[currentSelectedIndex]->selection(false);
            break;
        }

        currentSelectedIndex = idx;
        currentSelectedType = tpe;

        emit sceneSelectionChanged(currentSelectedType);

        if(tpe == 1)
        {
            AllLeafClouds[currentSelectedIndex]->selection(true);
            cloudPos->x = AllLeafClouds[currentSelectedIndex]->getCenter().x;
            cloudPos->y = AllLeafClouds[currentSelectedIndex]->getCenter().y;
            cloudPos->z = AllLeafClouds[currentSelectedIndex]->getCenter().z;

            cloudDimensions->w = AllLeafClouds[currentSelectedIndex]->getCloudRads().w;
            cloudDimensions->x = AllLeafClouds[currentSelectedIndex]->getCloudRads().x;
            cloudDimensions->y = AllLeafClouds[currentSelectedIndex]->getCloudRads().y;
            cloudDimensions->z = AllLeafClouds[currentSelectedIndex]->getCloudRads().z;

            cloudParams->x = AllLeafClouds[currentSelectedIndex]->size();
            cloudParams->y = AllLeafClouds[currentSelectedIndex]->radius();
            cloudParams->z = AllLeafClouds[currentSelectedIndex]->influence();
            cloudParams->w = 1234567.0;

            cloudShapetoEdit = AllLeafClouds[currentSelectedIndex]->getCloudType();
            updateGizmoPos();

            emit xPosChanged((int)cloudPos->x * 100);
            emit yPosChanged((int)cloudPos->y * 100);
            emit zPosChanged((int)cloudPos->z * 100);

            emit cloudXRaChanged(cloudDimensions->x * 100);
            emit cloudYRaChanged(cloudDimensions->y * 100);
            emit cloudZRaChanged(cloudDimensions->z * 100);
            emit cloudSizChanged((int)cloudDimensions->w * 100);

            emit cloudNumChanged((int)cloudParams->x);
            emit cloudKilChanged((int)cloudParams->y * 100);
            emit cloudInfChanged((int)cloudParams->z * 100);
            //emit cloudChanged(cloudPos->x);

            emit cloudShpChanged(cloudShapetoEdit);
        }

        else if(tpe == 2)
        {
//            AllBoxes[currentSelectedIndex]->selection(true);
//            boxPos->x = AllBoxes[currentSelectedIndex]->getPosition().x;
//            boxPos->y = AllBoxes[currentSelectedIndex]->getPosition().y;
//            boxPos->z = AllBoxes[currentSelectedIndex]->getPosition().z;

//            boxSize->x = AllBoxes[currentSelectedIndex]->getSize().x;
//            boxSize->y = AllBoxes[currentSelectedIndex]->getSize().y;
//            boxSize->z = AllBoxes[currentSelectedIndex]->getSize().z;
//            boxSize->w = AllBoxes[currentSelectedIndex]->getSize().w;

            AllObstructions[currentSelectedIndex]->selection(true);
            boxPos->x = AllObstructions[currentSelectedIndex]->getPosition().x;
            boxPos->y = AllObstructions[currentSelectedIndex]->getPosition().y;
            boxPos->z = AllObstructions[currentSelectedIndex]->getPosition().z;

            boxSize->x = AllObstructions[currentSelectedIndex]->getSize().x;
            boxSize->y = AllObstructions[currentSelectedIndex]->getSize().y;
            boxSize->z = AllObstructions[currentSelectedIndex]->getSize().z;
            boxSize->w = AllObstructions[currentSelectedIndex]->getSize().w;
            updateGizmoPos();


            emit xPosChanged((int)boxPos->x * 100);
            emit yPosChanged((int)boxPos->y * 100);
            emit zPosChanged((int)boxPos->z * 100);

            emit lenSizeChanged(boxSize->x * 100);
            emit widSizeChanged(boxSize->y * 100);
            emit hgtSizeChanged(boxSize->z * 100);
            emit axiSizeChanged(boxSize->w);//(int)boxSize->w == 2 ? 2 : 0);
        }
    }
    else
    {
        currentGizmoIndex = idx;
        currentGizmoType = tpe;
        lineEnd = tempPoint;
    }
    updateGL();
    return 1;
}

// Slots to set the variables
void GLWidget::setColValue(int val) { colValue = val; }
void GLWidget::setCntValue(int val) { cntValue = val; }
void GLWidget::setShpValue(int val) { shpValue = val; }
void GLWidget::setInfValue(int val) { infValue = (double)val/100.0; }
void GLWidget::setKilValue(int val) { kilValue = (double)val/100.0; }
void GLWidget::setXRaValue(int val) { xRaValue = (double)val/100.0; }
void GLWidget::setYRaValue(int val) { yRaValue = (double)val/100.0; }
void GLWidget::setZRaValue(int val) { zRaValue = (double)val/100.0; }
void GLWidget::setCSzValue(int val) { cSzValue = (double)val/100.0; }
void GLWidget::setSedValue(int val) { sedValue = val; }
void GLWidget::createNewCloud(bool)
{
    AllLeafClouds.push_back(new LeafCloud(colValue, cntValue, shpValue, infValue,
                                          kilValue, xRaValue, yRaValue, zRaValue,
                                          cSzValue, sedValue, AllLeafClouds.size()-1));
    updateGL();
}


void GLWidget::setSizeLen(int val)
{
    if(currentSelectedType == 2)
    {
        boxSize->x = (double)val/100.0;
        //AllBoxes[currentSelectedIndex]->changeSize(boxSize);
        AllObstructions[currentSelectedIndex]->changeSize(boxSize);
        updateGL();
    }
}

void GLWidget::setSizeWid(int val)
{
    if(currentSelectedType == 2)
    {
        boxSize->y = (double)val/100.0;
        //AllBoxes[currentSelectedIndex]->changeSize(boxSize);
        AllObstructions[currentSelectedIndex]->changeSize(boxSize);
        updateGL();
    }
}

void GLWidget::setSizeHgt(int val)
{
    if(currentSelectedType == 2)
    {
        boxSize->z = (double)val/100.0;
        //AllBoxes[currentSelectedIndex]->changeSize(boxSize);
        AllObstructions[currentSelectedIndex]->changeSize(boxSize);
        updateGL();
    }
}

void GLWidget::setSizeAxi(int val)
{
    if(currentSelectedType == 2)
    {
            boxSize->w = val;// == 1 ? 2 : 0;
            double t1 = boxSize->x;//, t2 = boxSize->y;
            boxSize->x = boxSize->y;//val == 2 ? t1 : t2;
            boxSize->y = t1;//val == 0 ? t1 : t2;
            emit lenSizeChanged(boxSize->x * 100);
            emit widSizeChanged(boxSize->y * 100);

        //AllBoxes[currentSelectedIndex]->changeSize(boxSize);
        AllObstructions[currentSelectedIndex]->changeSize(boxSize);
        updateGL();
    }
}

void GLWidget::setXPos(int valu)
{
    double val = (double)valu/100.0;
    if(currentSelectedType == 0)
        trunkStartPos->x = val;
    else if(currentSelectedType == 1)
    {
        cloudPos->x = val;
        AllLeafClouds[currentSelectedIndex]->changeCenter(cloudPos);
        updateGizmoPos();
    }
    else if(currentSelectedType == 2)
    {
        boxPos->x = val;
        //AllBoxes[currentSelectedIndex]->changePosition(boxPos);
        AllObstructions[currentSelectedIndex]->changePosition(boxPos);
        updateGizmoPos();
    }
    emit xPosChanged(valu);
    updateGL();
}

//Tess if two poits are visible, ith a bing the origin and b the destin
bool GLWidget::canSee(Point3* a, Point3* b)
{
    Ray newRay(*a, *b);
    Point3 temp(0, 0, 0);

//    for(int i = 0; i < AllBoxes.size(); ++i)
//    {
//        if(AllBoxes[i]->intersectionTest(newRay, temp))
//            return false;
//    }

    for(int i = 0; i < AllObstructions.size(); ++i)
    {
        if(AllObstructions[i]->intersectionTest(newRay, temp))
            return false;
    }

    return true;
}

void GLWidget::setYPos(int valu)
{
    double val = (double)valu/100.0;
    if(currentSelectedType == 0)
        trunkStartPos->y = val;
    else if(currentSelectedType == 1)
    {
        cloudPos->y = val;
        AllLeafClouds[currentSelectedIndex]->changeCenter(cloudPos);
        updateGizmoPos();
    }
    else if(currentSelectedType == 2)
    {
        boxPos->y = val;
        //AllBoxes[currentSelectedIndex]->changePosition(boxPos);
        AllObstructions[currentSelectedIndex]->changePosition(boxPos);
        updateGizmoPos();
    }
    emit yPosChanged(valu);
    updateGL();
}

void GLWidget::setZPos(int valu)
{
    double val = (double)valu/100.0;
    if(currentSelectedType == 0)
        trunkStartPos->z = val;
    else if(currentSelectedType == 1)
    {
        cloudPos->z = val;
        AllLeafClouds[currentSelectedIndex]->changeCenter(cloudPos);
        updateGizmoPos();
    }
    else if(currentSelectedType == 2)
    {
        boxPos->z = val;
        //AllBoxes[currentSelectedIndex]->changePosition(boxPos);
        AllObstructions[currentSelectedIndex]->changePosition(boxPos);
        updateGizmoPos();
    }
    emit zPosChanged(valu);
    updateGL();
}

void GLWidget::deleteObject(bool)
{
    if(currentSelectedType == 1)
    {
        LeafCloud *toRemove = AllLeafClouds[currentSelectedIndex];
        AllLeafClouds.erase(AllLeafClouds.begin()+currentSelectedIndex);
        delete toRemove;
    }
    else if(currentSelectedType == 2)
    {
        //AABB *toRemove = AllBoxes[currentSelectedIndex];
        //AllBoxes.erase(AllBoxes.begin()+currentSelectedIndex);
        Object *toRemove = AllObstructions[currentSelectedIndex];
        AllObstructions.erase(AllObstructions.begin()+currentSelectedIndex);
        delete toRemove;
    }

    currentSelectedType = 0;
    emit sceneSelectionChanged(currentSelectedType);
    forceUpdateScene = true;
    updateGL();
}


// Utility Functions to help in calculations for tree growth
bool GLWidget::boxIntersect(Octnode& cNode, double rad, Point3 location)
{
    float dmin = 0;
    float r2 = rad * rad;

    double bmin[3] = {cNode.A().x, cNode.A().y, cNode.A().z};
    double bmax[3] = {cNode.B().x, cNode.B().y, cNode.B().z};
    double C[3] = {location.x, location.y, location.z};

    for(int i = 0; i < 3; ++i)
    {
        if(C[i] < bmin[i])
            dmin += pow((C[i] - bmin[i]), 2.0);
        else if(C[i] > bmax[i])
            dmin += pow((C[i] - bmax[i]), 2.0);
    }

    if(dmin <= r2)
        return true;

    return false;
}

bool GLWidget::killFunc(Octnode& cNode, int cLeaf, LeafCloud* leafCloud)
{
    if(!(boxIntersect(cNode, (*leafCloud)[cLeaf].radius(), (*leafCloud)[cLeaf].location())))
        return false;

    if(cNode.isLeaf)
    {
        vector<Branch*> cList = cNode.myPoints;
        for(uint i = 0; i < cList.size(); ++i)
        {
            if((*leafCloud)[cLeaf].inme(cList[i]->endNodeTest))
            {
                return true;
            }
        }
        return false;
    }

    for(int i = 0; i < 8; ++i)
    {
        bool ini = killFunc(*cNode.child[i], cLeaf, leafCloud);
        if(ini)
            return true;
    }

    return false;
}

bool GLWidget::getBranchesInLeaf(vector<Branch*>& totBranches, LeafCloud* leafCloud, Octnode& cNode, int cLeaf, bool& stuck, int ageLimit)
{
    if(!(boxIntersect(cNode, (*leafCloud)[cLeaf].influence(), (*leafCloud)[cLeaf].location())))
        return false;

    bool added = false;
    if(cNode.isLeaf)
    {
        vector<Branch*> cList = cNode.myPoints;
        for(uint i = 0; i < cList.size(); ++i)
        {
            if((!growOption) || (cList[i]->visibility() && growOption))
            {
                if(cList[i]->age() >= ageLimit && cList[i]->children.size() <=4)
                {
                    if((*leafCloud)[cLeaf].influence(cList[i]->endNodeTest))
                    {
                        totBranches.push_back(cList[i]);
                    }
                }
            }
        }
        if(totBranches.size() > 0)
            return true;
        else
            return false;
    }

    for(int i = 0; i < 8; ++i)
    {
        if(getBranchesInLeaf(totBranches, leafCloud, *cNode.child[i], cLeaf, stuck, ageLimit))
            added = true;
    }

    return added;
}

Branch* GLWidget::findClosestBranch(LeafCloud* leafCloud, vector<Branch*>& totBranches, int cLeaf)
{
    double cDist = DM;
    int b = -1;
    for(uint i = 0; i < totBranches.size(); ++i)
    {
        double md = (*leafCloud)[cLeaf].location().distanceToSquared(totBranches[i]->endNodeTest);
        if(nodeToBranchOpt)
        {
            Ray PtB = Ray((*leafCloud)[cLeaf].location(), *(totBranches[i]->endNodeTest));
            for(uint p = 0; p < AllObstructions.size(); p++)
            {
                Point3 t(0, 0, 0);
                if(AllObstructions[p]->intersectionTest(PtB, t))
                {
                    //this part is original
                    if((*leafCloud)[cLeaf].location().distanceToSquared(t) <= md)
                        break;

                    if(md < cDist)
                    {
                        cDist = md;
                        b = i;
                    }
                }
                else
                {
                    //double md = (*leafCloud)[cLeaf].location().distanceToSquared(totBranches[i]->endNodeTest);
                    if(md < cDist)
                    {
                        cDist = md;
                        b = i;
                    }
                }
            }
        }
                //double md = (*leafCloud)[cLeaf].location().distanceToSquared(totBranches[i]->endNodeTest);
        else
        {
            if(md < cDist)
            {
                cDist = md;
                b = i;
            }
        }
    }

    if(b == -1)
        return new Branch(new Point3(), new Point3(), -1);
    else
        return totBranches[b];
}

void GLWidget::setTrunk(bool)
{
    //uncomment to lock to one trunk creation to one branch.
    if(AllBranches.size() != 0)
        return;

    int generation = 0;
    AllBranches.push_back(new Branch(new Point3(trunkStartPos->x, trunkStartPos->y, trunkStartPos->z),
                                     new Point3(trunkStartPos->x, trunkStartPos->y+1, trunkStartPos->z),
                                     generation++));
    AllBranches[0]->isparent(true);
    AllBranches[0]->setroot(true);
    AllBranches[0]->me = NULL;
    Octree.push(AllBranches[0]);

    //initializes a list of branches
    for(int i = 1; i < trunkHeight; ++i)
    {
        AllBranches.push_back(new Branch(AllBranches[i-1]->endNodeTest,
                                         new Point3(trunkStartPos->x, trunkStartPos->y+i+1, trunkStartPos->z),
                                         generation++));
        AllBranches[i-1]->isparent(true);
        AllBranches[i-1]->children.push_back(AllBranches[i]);

        AllBranches[i]->me = AllBranches[i-1];

        Octree.push(AllBranches[i]);
    }
    currentAgeLimit = generation - 1;
    maxAgeLimit = currentAgeLimit;

    emit ageLimitChanged(0, currentAgeLimit);

    updateGL();
}

void GLWidget::updateGizmoPos()
{
    Point3 temp(0, 0, 0);
    switch(currentSelectedType)
    {
        case 1:
            temp = AllLeafClouds[currentSelectedIndex]->getCenter();
            break;
        case 2:
            //temp = AllBoxes[currentSelectedIndex]->getPosition();
            temp = AllObstructions[currentSelectedIndex]->getPosition();
            break;
    }

    for(int i = 0; i < 3; ++i)
    {
        gizmo[i]->changePosition(&temp);
    }
}

void GLWidget::drawGizmo()
{
    for(int i = 0; i < 3; ++i)
    {
        gizmo[i]->drawGiz(true, false);
    }
}

void GLWidget::setTrunkHeight(int h)
{
    trunkHeight = h;
}


void GLWidget::computeBranchRadii()
{
    Branch* next;
    int totChild = 0;

    for(uint i = 0; i < AllBranches.size(); ++i)
    {
        AllBranches[i]->clearRadius();
    }

    for(uint i = 0; i < AllBranches.size(); ++i)
    {
        if(AllBranches[i]->isparent())
            continue;

        next = AllBranches[i];
        while(next->me)
        {
            if(!next->visibility() && next->me->visibility())
                if(next->me->radius() > 0.0)
                    break;

            if(next->me->visibility())
            {
                next->radius(currentTreeGirth, totChild++);
                //next->me->radius(currentTreeGirth);
            }
            next = next->me;
        }
        totChild = 0;
    }
}

void GLWidget::updateGirth(int g)
{
    currentTreeGirth = (double)g/100.0;
    computeBranchRadii();
    updateGL();
}

void GLWidget::setCloudXRa(int val)
{
    cloudDimensions->x = ((double)val/100.0);
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudYRa(int val)
{
    cloudDimensions->y = ((double)val/100.0);
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudZRa(int val)
{
    cloudDimensions->z = ((double)val/100.0);
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudSiz(int val)
{
    cloudDimensions->w = (double)val/100.0;
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudNum(int val)
{
    cloudParams->x = (double)val;
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudKil(int val)
{
    cloudParams->y = (double)val/100.0;
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudInf(int val)
{
    cloudParams->z = (double)val/100.0;
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setCloudShape(int val)
{
    cloudShapetoEdit = val;
    AllLeafClouds[currentSelectedIndex]->changeParams(cloudDimensions, cloudParams, cloudShapetoEdit);
    updateGL();
}

void GLWidget::setGraviTrop(double val)
{
    graviTropVal = val;
}

void GLWidget::drawBranches(bool wire)
{
    bool hasMoved = false;

//    for(int i = 0; i < AllBoxes.size(); ++i)
//    {
//        hasMoved = AllBoxes[i]->hasMoved();
//        if(hasMoved)
//            break;
//    }

    for(int i = 0; i < AllObstructions.size(); ++i)
    {
        hasMoved = AllObstructions[i]->hasMoved();
        if(hasMoved)
            break;
    }

    if(hasMoved || lastSize != AllBranches.size() || forceUpdateScene)
    {
        forceUpdateScene = false;
        for(uint i = 0; i < AllBranches.size(); ++i)
        {
            if(AllBranches[i]->age() >= currentAgeLimit)
            {
                AllBranches[i]->visibility(false);
                AllBranches[i]->markTwo(true);
            }
            else
                AllBranches[i]->visibility(true);

//            for(int j = 0; j < AllBoxes.size(); ++j)
//            {
//                if(AllBoxes[j]->intersectionTest(AllBranches[i]->endNodeTest))
//                {
//                    AllBranches[i]->visibility(false);
//                    AllBranches[i]->markTwo(true);
//                    break;
//                }
//            }

            for(int j = 0; j < AllObstructions.size(); ++j)
            {
                if(AllObstructions[j]->intersectionTest(AllBranches[i]->endNodeTest))
                {
                    AllBranches[i]->visibility(false);
                    AllBranches[i]->markTwo(true);
                    break;
                }
            }
        }

        for(uint i = 0; i < AllBranches.size(); ++i)
        {
            if(AllBranches[i]->mark())
            {
                Branch::switchSisibilityOff(AllBranches[i]);
            }
        }

        computeBranchRadii();
    }

    if(drawTreeF)
    {
        for(uint j = 0; j < AllBranches.size(); ++j)
        {
            AllBranches[j]->markTwo(false);
            if((AllBranches[j]->age() <= currentAgeLimit) && AllBranches[j]->visibility())
            {
                glPushMatrix();
                glLineWidth( 1.5f );
                glBegin(GL_LINES);
                    glColor3f(0.055f, 0.055f, 0.055f);
                    glEnable( GL_LINE_SMOOTH );
                    glVertex3f(AllBranches[j]->endNodeTest->x, AllBranches[j]->endNodeTest->y, AllBranches[j]->endNodeTest->z);
                    glVertex3f(AllBranches[j]->startNodeTest->x, AllBranches[j]->startNodeTest->y, AllBranches[j]->startNodeTest->z);
                glEnd();
                glPopMatrix();
            }
         }
    }
    else
    {
        for(uint j = 0; j < AllBranches.size(); ++j)
        {
            AllBranches[j]->markTwo(false);
            if((AllBranches[j]->age() <= currentAgeLimit) && AllBranches[j]->visibility())
            {
                double radBase = /*AllBranches[j]->radius(); //*/sqrt(AllBranches[j]->radius());
                double radTop = 0.0;

                //if(AllBranches[j]->children.size() == 1)
                if(AllBranches[j]->children.size() > 0)
                    radTop = /*AllBranches[j]->children[0]->radius(); //*/sqrt(AllBranches[j]->children[0]->radius());
                else
                    radTop = radBase;

                glPushMatrix();

                glColor3f(0.055f, 0.055f, 0.055f);

                glTranslated(AllBranches[j]->startNodeTest->x,AllBranches[j]->startNodeTest->y,AllBranches[j]->startNodeTest->z);
                glRotated(AllBranches[j]->fangle,AllBranches[j]->rot.x,AllBranches[j]->rot.y,AllBranches[j]->rot.z);

                gluQuadricOrientation(quadricCyl, GLU_OUTSIDE);
                gluCylinder(quadricCyl, radBase, radTop, /*p.length()*/1.0, 9.0, 3.0);
                glPopMatrix();
            }
        }
    }
}

void GLWidget::setStepSize(int val)
{
    stepSize = val;
}

void GLWidget::setAgeLimit(int val)
{
    currentAgeLimit = val;
    forceUpdateScene = true;
    updateGL();
}

void GLWidget::setGrowLimit(int val)
{
    currentGrowLimit = val > 4 ? val : 5;
}

void GLWidget::switchToDrawOptions(bool)
{
    emit sceneSelectionChanged(3);
}

void GLWidget::switchToMainOptions(bool)
{
    switch(currentSelectedType)
    {
        case 1:
            AllLeafClouds[currentSelectedIndex]->selection(false);
            break;
        case 2:
            //AllBoxes[currentSelectedIndex]->selection(false);
            AllObstructions[currentSelectedIndex]->selection(false);
            break;
    }

    currentSelectedIndex = 0;
    currentSelectedType = 0;

    emit sceneSelectionChanged(0);    
    updateGL();
}

void GLWidget::drawAABB(Octnode* octs)
{
    Point3 min = octs->A();
    Point3 max = octs->B();

    Vector3 v0(min.x, min.y, min.z);
    Vector3 v1(max.x, min.y, min.z);
    Vector3 v2(max.x, max.y, min.z);
    Vector3 v3(min.x, max.y, min.z);
    Vector3 v4(min.x, min.y, max.z);
    Vector3 v5(max.x, min.y, max.z);
    Vector3 v6(max.x, max.y, max.z);
    Vector3 v7(min.x, max.y, max.z);

    glPushMatrix();
    glLineWidth(0.5f);
    glBegin(GL_LINES);
        glColor3f(1.0f/(float)(octs->size()+0.05), 0.00f, 0.00f);
        glEnable( GL_LINE_SMOOTH );
        glVertex3f(v0.x, v0.y, v0.z); glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v0.x, v0.y, v0.z); glVertex3f(v4.x, v4.y, v4.z);
        glVertex3f(v0.x, v0.y, v0.z); glVertex3f(v3.x, v3.y, v3.z);
        glVertex3f(v6.x, v6.y, v6.z); glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v6.x, v6.y, v6.z); glVertex3f(v7.x, v7.y, v7.z);
        glVertex3f(v6.x, v6.y, v6.z); glVertex3f(v5.x, v5.y, v5.z);
        glVertex3f(v5.x, v5.y, v5.z); glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v5.x, v5.y, v5.z); glVertex3f(v4.x, v4.y, v4.z);
        glVertex3f(v3.x, v3.y, v3.z); glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z); glVertex3f(v7.x, v7.y, v7.z);
        glVertex3f(v4.x, v4.y, v4.z); glVertex3f(v7.x, v7.y, v7.z);
        glVertex3f(v2.x, v2.y, v2.z); glVertex3f(v1.x, v1.y, v1.z);
    glEnd();
    glPopMatrix();


    if(octs->hasChild)
    {
        for(int i = 0; i<8; ++i)
        {
            drawAABB(octs->child[i]);
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Delete)
    {
        deleteObject(true);
    }
    else if (e->key() == Qt::Key_X)
    {
        Camera = CCamera();
        axisCamera = CCamera(Vector3 (0.0, 0.0, 10.0), Camera.getViewPoint(),
                             Vector3 (1.0, 0.0, 0.0), Camera.getUp());
        updateGL();
    }
    else if (e->key() == Qt::Key_A)
    {
        Camera = CCamera(Vector3(250, 15, 0), Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0));
        axisCamera = CCamera(Vector3(10, 0.5, 0), Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0));
        updateGL();
    }
    else if (e->key() == Qt::Key_D)
    {
        Camera = CCamera(Vector3(-250, 15, 0), Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
        axisCamera = CCamera(Vector3(-10, 0.5, 0), Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
        updateGL();
    }
    else if (e->key() == Qt::Key_W)
    {
        Camera = CCamera(Vector3(0, 15, -250), Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, 1, 0));
        axisCamera = CCamera(Vector3(0, 0.5, -10), Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, 1, 0));
        updateGL();
    }
    else if (e->key() == Qt::Key_S)
    {
        Camera = CCamera(Vector3(0, 250, 0), Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, 0, 1));
        axisCamera = CCamera(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(-1, 0, 0), Vector3(0, 0, 1));
        updateGL();
    }
    else if (e->key() == Qt::Key_J)
    {
        drawABbox = !drawABbox;
        updateGL();
    }
    else if (e->key() == Qt::Key_P)
    {
        pruneHiddenTreeBranches(true);
        updateGL();
    }
    else if (e->key() == Qt::Key_F)
    {
        Vector3 t(0, 0, 0);

        if(currentSelectedType == 0)
        {
            t = Vector3(0, 0, 0) - Camera.getViewPoint();
        }
        else
        {
            switch(currentSelectedType)
            {
            case 1:
                t = Point3::PtV(AllLeafClouds[currentSelectedIndex]->getCenter()) - Camera.getViewPoint();
                break;
            case 2:
                t = Point3::PtV(AllObstructions[currentSelectedIndex]->getPosition()) - Camera.getViewPoint();
                break;
            }
        }

        Camera.Move(t);
        updateGL();
    }
}

void GLWidget::growTreeOriginal(bool)
{
    if(AllBranches.size() == 0 || AllLeafClouds.size() == 0)
        return;

//    int leafStep = 0;
    int totalLeaves = 0;
    int totalLeavesLeft = 0;
    LeafCloud *leafCloud;

    for(uint i = 0; i < AllLeafClouds.size(); ++i)
    {
        totalLeaves += AllLeafClouds[i]->size();
        totalLeavesLeft += AllLeafClouds[i]->remaining();

    }

    int dead = 0;
    int turns = 0;
    lastDead = 0;
    lastSize = AllBranches.size();
    Branch* cBranch;
    vector<Branch*> cBranches;

    if(stepSize > totalLeavesLeft)
        stepSize = totalLeavesLeft;

    QTime *timer = new QTime;
    timer->start();
    //This is the main loop that will continue to run until no more leaves  are in the list.
    while((dead < stepSize))//  && (stepSize != 0))
    { //dead keeps count of dead leaves. Each time a leaf is removed, it is incremented.
        uint p = 0;
        uint k = 0;
        int pos = 0;

        for(uint i = 0; i < totalLeaves; ++i , ++k)
        {
            if(k >= AllLeafClouds[pos]->size())
            {
                k = 0; ++pos;
                if(pos >= AllLeafClouds.size())
                    break;
            }

            leafCloud = AllLeafClouds[pos];
//            for(uint i=0; i < AllLeafClouds[pos]->size(); ++i)
//            { //This is the first set of loops, for each leaf, check against each branch
                bool close = false;
                bool stuck = false;

                if((*leafCloud)[k].active() && nodeInBlockOpt)
                {
                            for(uint p = 0; p < AllObstructions.size(); p++)
                            {
                                Point3 t = (*leafCloud)[k].location();
                                if(AllObstructions[p]->intersectionTest(&t))
                                {
                                        ++dead;
                                        (*leafCloud)[k].kill();
                                        break;
                                }
                            }
                }
                if((*leafCloud)[k].active() && nodeToCenterOpt)
                {
                    Ray PtC = Ray((*leafCloud)[k].location(), leafCloud->getCenter());
                    for(uint p = 0; p < AllObstructions.size(); p++)
                    {
                        Point3 t(0, 0, 0);
                        if(AllObstructions[p]->intersectionTest(PtC, t))
                        {
                            if((*leafCloud)[k].location().distanceToSquared(t) <= (*leafCloud)[k].location().distanceToSquared(leafCloud->getCenter()))
                            {
                                ++dead;
                                (*leafCloud)[k].kill();
                                break;
                            }
                        }
                    }
                }

                //if((*AllLeafClouds[pos])[k].active())
                if((*leafCloud)[k].active())
                {
                    close = getBranchesInLeaf(cBranches, AllLeafClouds[pos], Octree, k, stuck, maxAgeLimit - currentGrowLimit);

                    if(close)
                    {
                        cBranch = findClosestBranch(AllLeafClouds[pos], cBranches, k);

                        if(cBranch->age() == -1)
                            continue;

                        //if((*AllLeafClouds[pos])[k].checkBranch(cBranch) > 5)
                        if((*leafCloud)[k].checkBranch(cBranch) > 3)
                        {
                            ++dead;
                            //(*AllLeafClouds[pos])[k].kill();
                            (*leafCloud)[k].kill();

                            continue;
                        }

                        //cBranch->sum((*AllLeafClouds[pos])[k].location());
                        cBranch->sum((*leafCloud)[k].location());
                        cBranch->mark(true);
                        cBranch->ang(true);
                        (*leafCloud)[k].lifeExtend();
                        close = false;
                    }
                    else
                    {
                        if(true){}//(*leafCloud)[k].lifeDegrade())
                            //++dead;
                    }
                }
                cBranches.clear();
            }

            for(uint j= 0; j < AllBranches.size(); ++j)
            { //here if a tree has has been marked, its the closest to one of the leaves, and so we add B to it
                if(AllBranches[j]->mark())
                {
                    AllBranches[j]->mark(false);
                    Vector3 newDire = AllBranches[j]->div();

                    int cAge = AllBranches[j]->age()+1;
                    if(cAge > maxAgeLimit)
                        maxAgeLimit = cAge;

                    if(graviTropVal > 0.0)
                    {
                        double ndist = AllBranches[j]->endNodeTest->distanceTo(Point3(0, AllBranches[j]->endNodeTest->y, 0));
                        if(ndist <= 0.0){}
                        else
                        {
                            ndist = ndist/AllLeafClouds[pos]->getCloudRads().w;
                            Vector3 newDirGravi = Vector3(0, -1, 0) * ndist * graviTropVal;
                            newDire = (newDire + newDirGravi); newDire.normalize();// / 2;
                        }
                    }

                    Branch* newBranch = new Branch(AllBranches[j]->endNodeTest,
                                                   new Point3(AllBranches[j]->endNodeTest->x + newDire.x, AllBranches[j]->endNodeTest->y + newDire.y, AllBranches[j]->endNodeTest->z + newDire.z),
                                                   cAge);
                    newBranch->me = AllBranches[j];
                    AllBranches[j]->isparent(true);
                    AllBranches[j]->children.push_back(newBranch);
                    AllBranches.push_back(newBranch);
                    Octree.push(newBranch);
                    //++check;
                    AllBranches[j]->ang(false);
                }
            }

            k = 0;
            pos = 0;
            for(uint h = 0; h < totalLeaves; ++h, ++k)
            { //we now check again against all leaves if any new branches are in the  kill radius
                //if((*AllLeafClouds[pos])[h].active())
                if(k >= AllLeafClouds[pos]->size())
                {
                    k = 0; ++pos;
                    if(pos >= AllLeafClouds.size())
                        break;
                }

                leafCloud = AllLeafClouds[pos];

                if((*leafCloud)[k].active())
                {
                    if(killFunc(Octree, k, AllLeafClouds[pos]))
                    {
                        //(*AllLeafClouds[pos])[h].kill();
                        (*leafCloud)[k].kill();
                        ++dead;
                    }
                }
            }
            //if((dead >= 1750))//||(check >= 1050))
            //break;



        if((lastDead == dead) && ((lastSize == AllBranches.size()) || turns > 50))
        {
            turns = 0;
            for(uint i = 0; i < AllLeafClouds.size(); ++i)
            {
                    delete AllLeafClouds[i];
                    AllLeafClouds.erase(AllLeafClouds.begin() + i);
            }
            break;
        }
        else
        {
            lastDead = dead;
            lastSize = AllBranches.size();
           // turns++;
        }
    }

    qDebug() << timer->elapsed();

    for(uint i = 0; i < AllLeafClouds.size(); ++i)
    {
        if(AllLeafClouds[i]->remaining() == 0)
        {
            delete AllLeafClouds[i];
            AllLeafClouds.erase(AllLeafClouds.begin() + i);
        }
    }

    emit ageLimitChanged(0, maxAgeLimit);
    updateGL();
}

void GLWidget::setBoxLen(int val)
{
    boxDimensions->x = (double)val/100.0;
}

void GLWidget::setBoxWid(int val)
{
    boxDimensions->z = (double)val/100.0;
}

void GLWidget::setBoxHgt(int val)
{
    boxDimensions->y = (double)val/100.0;
}

void GLWidget::setBoxAxis(int val)
{
    boxDimensions->w = val;
}

void GLWidget::addBlock(bool)
{
//    AllBoxes.push_back(new AABB(new Point3(15, 0, 0), boxDimensions->x, boxDimensions->z,
//                                boxDimensions->y, boxDimensions->w));
    AllObstructions.push_back(new AABB(new Point3(15, 0, 0), boxDimensions->x, boxDimensions->z,
                                boxDimensions->y, boxDimensions->w));
    updateGL();
}

void GLWidget::addEllipsoid(bool)
{
    AllObstructions.push_back(new Ellipsoid(new Point3(15, 0, 0), boxDimensions->x, boxDimensions->z,
                                boxDimensions->y, boxDimensions->w));
    updateGL();
}

void GLWidget::toggleAABB(bool)
{
    drawABbox = !drawABbox;
    updateGL();
}

Vector3 toEuler(double x,double y,double z,double angle)
{
    double val = PI/180;
    double s=sin(angle * val);
    double c=cos(angle * val);
    double t=1-c;
    Vector3 pop(0, 0, 0);
    //  if axis is not already normalised then uncomment this
     double magnitude = sqrt(x*x + y*y + z*z);
     if (magnitude==0)
         return pop;
     x /= magnitude;
     y /= magnitude;
     z /= magnitude;
    if ((x*y*t + z*s) > 0.998) { // north pole singularity detected
        pop.y = 2*atan2(x*sin((angle/2) * val),cos((angle/2) * val));
        pop.z = PI/2;
        pop.x = 0;
        return pop;
    }
    if ((x*y*t + z*s) < -0.998) { // south pole singularity detected
        pop.y = -2*atan2(x*sin((angle/2) * val),cos((angle/2) * val));
        pop.z = -PI/2;
        pop.x = 0;
        return pop;
    }
    pop.y = atan2(y * s- x * z * t , 1 - (y*y+ z*z ) * t) / val;
    pop.z = asin(x * y * t + z * s) / val;
    pop.x = atan2(x * s - y * z * t , 1 - (x*x + z*z) * t) / val;

    return pop;
}

void GLWidget::createPythonScript(bool)
{
    int cylName = 0;
    Vector3 eu(0, 0, 0);

    QString fn = QFileDialog::getSaveFileName(this, "Export a Python Script", "", ".py");
    fn += QString(".py");
    QFile file(fn);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "import maya.cmds as cmds\n\n";

    //out << "def createMyTree():\n";
    for(int i = 0; i < AllBranches.size(); ++i)
    {
        if(!AllBranches[i]->visibility())
            continue;

        //eu = toEuler(AllBranches[i]->rot.x, AllBranches[i]->rot.y, AllBranches[i]->rot.z, AllBranches[i]->fangle);
        eu = quaternion::from_axis_angle(AllBranches[i]->rot, AllBranches[i]->fangle).normalized().euler_angles(false);
        out << "cmds.polyCylinder(n=\'treeCyl_" << cylName << "\', sx=15, sy=5, sz=5, h=1, r=" << sqrt(AllBranches[i]->radius()) << ")\n";
        out << "cmds.move(0, 0.5, 0)\n";
        out << "cmds.move(0, 0, 0, \"treeCyl_" << cylName << ".scalePivot\",\"treeCyl_" << cylName << ".rotatePivot\", absolute=True)\n";
        out << "cmds.rotate( 90, 0, 0)\n";
        out << "cmds.makeIdentity(apply=True, t=1, r=1, s=1, n=0)\n";
        out << "cmds.move( " << AllBranches[i]->startNodeTest->x << ", " << AllBranches[i]->startNodeTest->y << ", " << AllBranches[i]->startNodeTest->z << " )\n";
        out << "cmds.rotate( " << eu.x << ", " << eu.y << ", " << eu.z << ")\n";
        out << "# Axis: " << AllBranches[i]->fangle << ", Angle: (" << AllBranches[i]->rot.x << ", " << AllBranches[i]->rot.y << ", " << AllBranches[i]->rot.z << ")\n";
        out << "cmds.group( \'treeCyl_" << cylName++ << "\', n='TreeGroup' )\n\n";
    }
    // optional, as QFile destructor will already do it:
    file.close();
}

bool /*GLWidget::*/branchVisi(Branch *b)
{
    bool t =!(b->visibility());
    if(t)
        delete b;
    return t;
}

void GLWidget::pruneHiddenTreeBranches(bool)
{
    Octnode* next;
     stack<Octnode*> allkids;
     if(Octree.hasChild)
         for(int i = 0; i < 8; ++i)
         {
             allkids.push(Octree.child[i]);
         }
     else
         Octree.myPoints.clear();
     Octree.size(false);

     while(!allkids.empty())
     {
        next = allkids.top();
        if(next->hasChild)
        {
            for(int i = 0; i < 8; ++i)
            {
                allkids.push(next->child[i]);
            }

            next->hasChild = false;
        }
        else
        {
            allkids.pop();
            next->myPoints.clear();
            delete next;
        }
     }

     Point3 aa = Point3(-10000, -10000, -10000);
     Point3 bb = Point3(10000, 10000, 10000);
     Octree = Octnode(aa, bb, 200);

    for(uint i = 0; i < AllBranches.size(); ++i)
    {
        if(!(AllBranches[i]->visibility()))
            continue;

        for(uint j = 0; j < AllBranches[i]->children.size(); ++j)
        {
            if(!(AllBranches[i]->children[j]->visibility()))
                AllBranches[i]->children.erase(AllBranches[i]->children.begin() + j);
        }        

        Octree.push(AllBranches[i]);
    }

    AllBranches.erase(remove_if(AllBranches.begin(), AllBranches.end(), branchVisi), AllBranches.end());
}

void GLWidget::clearAllTreeBranches(bool)
{
    //Branch* toDel;
    for(int i = 0; i < AllBranches.size(); ++i)
    {
//        toDel = AllBranches[i];
//        AllBranches.erase()
        delete AllBranches[i];
    }
    AllBranches.clear();

    Octnode* next;
    stack<Octnode*> allkids;
    if(Octree.hasChild)
        for(int i = 0; i < 8; ++i)
        {
            allkids.push(Octree.child[i]);
        }
    else
        Octree.myPoints.clear();
    Octree.size(false);

    while(!allkids.empty())
    {
       next = allkids.top();
       if(next->hasChild)
       {
           for(int i = 0; i < 8; ++i)
           {
               allkids.push(next->child[i]);
           }

           next->hasChild = false;
       }
       else
       {
           allkids.pop();
           next->myPoints.clear();
           delete next;
       }
    }

    Point3 aa = Point3(-10000, -10000, -10000);
    Point3 bb = Point3(10000, 10000, 10000);
    Octree = Octnode(aa, bb, 200);
    updateGL();
}

void GLWidget::toggleDrawShell(bool val) { drawShell = val; updateGL(); }
void GLWidget::toggleDrawSolid(bool) { drawSolid = true; updateGL(); }
void GLWidget::toggleDrawTrans(bool) { drawSolid = false; updateGL(); }
void GLWidget::toggleDrawNodes(bool val) { drawNodes = val; updateGL(); }
void GLWidget::toggleDrawKillR(bool val) { drawKillR = val; updateGL(); }
void GLWidget::toggleDrawInflR(bool val) { drawImflR = val; updateGL(); }
void GLWidget::toggleDrawBoxes(bool val) { drawBoxes = val; updateGL(); }
void GLWidget::toggleDrawBoxWi(bool) { drawBoxWi = true; updateGL(); }
void GLWidget::toggleDrawBoxSo(bool) { drawBoxWi = false; updateGL(); }
void GLWidget::toggleDrawTreeF(bool) { drawTreeF = true; updateGL(); }
void GLWidget::toggleDrawTreeS(bool) { drawTreeF = false; updateGL(); }
void GLWidget::toggleNodeBranch(bool val) { nodeToBranchOpt = val; }
void GLWidget::toggleNodeCenter(bool val) { nodeToCenterOpt = val; }
void GLWidget::toggleNodeBlock(bool val) { nodeInBlockOpt = val; }
void GLWidget::toggleGrowOptionWire(bool)
{ growOption = true; }
void GLWidget::toggleGrowOptionSolid(bool)
{ growOption = false; }
