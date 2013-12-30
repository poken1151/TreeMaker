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
 *     The basic opengl widget class for use in Qt. Creats all the
 *     signals and slots needed to interface the opengl context
 *     with the UI elements.
 */
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <vector>


#include "leafCloud.h"
#include "object.h"
#include "aabb.h"
#include "ellipsoid.h"
#include "branch.h"
#include "vecmath.h"
#include "ray.h"
#include "octnode.h"
#include "quaternion.h"
#include "camera.h"

class QtLogo;

//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    CCamera Camera;
    CCamera axisCamera;
    Octnode Octree;
    vector<LeafCloud*> AllLeafClouds;
    vector<Object*> AllObstructions;
    vector<Branch*> AllBranches;
    vector<AABB*> gizmo;

    Vector3 gloPos;
    int leafStep;

    int objectAtPosition(const QPoint&);
    bool boxIntersect(Octnode&, double, Point3);
    bool killFunc(Octnode&, int, LeafCloud *leafCloud);
    bool getBranchesInLeaf(vector<Branch*>&, LeafCloud*, Octnode&, int, bool&, int);
    Branch* findClosestBranch(LeafCloud *leafCloud, vector<Branch*>&, int);

    GLUquadricObj *quadricCyl;

//! [0]

    //! [1]
    void drawAABB(Octnode *octs);
    void keyPressEvent(QKeyEvent *event);
    void drawAxis();
    void drawGizmo();
    void mouseReleaseEvent(QMouseEvent *event);
    Point3 virtualPlane(const QPoint &pos);
    void updateGizmoPos();
    void pruneHiddenTreeBranches(bool);
    Vector3 arcVec(const QPoint &pos);
public slots:
    void setColValue(int);
    void setCntValue(int);
    void setShpValue(int);
    void setInfValue(int);
    void setKilValue(int);
    void setXRaValue(int);
    void setYRaValue(int);
    void setZRaValue(int);
    void setCSzValue(int);
    void setSedValue(int);
    void createNewCloud(bool);
    void setTrunk(bool);

    void setXPos(int);
    void setYPos(int valu);
    void setZPos(int valu);
    void setSizeLen(int);
    void setSizeWid(int);
    void setSizeHgt(int);
    void setSizeAxi(int);
    void setTrunkHeight(int);    

    void setBoxLen(int);
    void setBoxWid(int);
    void setBoxHgt(int);
    void setBoxAxis(int);
    void addBlock(bool);
    void addEllipsoid(bool);
    void toggleAABB(bool);

    void setStepSize(int);
    void setAgeLimit(int);
    void deleteObject(bool);
    void growTreeOriginal(bool);
    void switchToDrawOptions(bool);
    void switchToMainOptions(bool);

    void toggleDrawShell(bool);
    void toggleDrawSolid(bool);
    void toggleDrawTrans(bool);
    void toggleDrawNodes(bool);
    void toggleDrawKillR(bool);
    void toggleDrawInflR(bool);
    void toggleDrawBoxes(bool);
    void toggleDrawBoxSo(bool);
    void toggleDrawBoxWi(bool);
    void toggleDrawTreeF(bool);
    void toggleDrawTreeS(bool);
    void toggleGrowOptionWire(bool);
    void toggleGrowOptionSolid(bool);
    void toggleNodeBranch(bool);
    void toggleNodeCenter(bool);
    void toggleNodeBlock(bool);

    void createPythonScript(bool);
    void clearAllTreeBranches(bool);
    void updateGirth(int);

    void setCloudXRa(int);
    void setCloudYRa(int);
    void setCloudZRa(int);
    void setCloudSiz(int);

    void setCloudNum(int);
    void setCloudKil(int);
    void setCloudInf(int);

    void setCloudShape(int);
    void setGraviTrop(double);
    void setGrowLimit(int val);

signals:
    void sceneSelectionChanged(int);
    void xPosChanged(int);
    void yPosChanged(int);
    void zPosChanged(int);
    void trunkCreated(double);
    void ageLimitChanged(int,int);
    void growLimitChanged(int);
    void lenSizeChanged(int);
    void widSizeChanged(int);
    void hgtSizeChanged(int);
    void axiSizeChanged(int);

    void cloudXRaChanged(int);
    void cloudYRaChanged(int);
    void cloudZRaChanged(int);
    void cloudSizChanged(int);

    void cloudNumChanged(int);
    void cloudKilChanged(int);
    void cloudInfChanged(int);

    void cloudShpChanged(int);

//! [1]

//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *);

//! [2]

//! [3]
private:
    //These items are used to set the default leaf cloud values.
    int colValue;
    int cntValue;
    int shpValue;
    double infValue;
    double kilValue;
    double xRaValue;
    double yRaValue;
    double zRaValue;
    double cSzValue;
    int sedValue;

    int trunkHeight;
    int stepSize;
    int maxAgeLimit;
    int currentAgeLimit;

    bool drawShell;
    bool drawTrans;
    bool drawSolid;
    bool drawNodes;
    bool drawKillR;
    bool drawImflR;
    bool drawBoxes;
    bool drawBoxWi;
    bool drawTreeF;
    bool growOption;
    bool forceUpdateScene;
    bool drawABbox;

    int lastSize;
    int lastDead;
    double currentTreeGirth;
    double graviTropVal;
    int currentGrowLimit;

    int cloudShapetoEdit;

    Vector4 *cloudParams;
    Vector4 *cloudDimensions;
    Vector4 *boxDimensions;
    Vector4 *boxSize;
    Point3 *boxPos;
    Point3 *cloudPos;
    Point3 *trunkStartPos;
    Point3 lineStart;
    Point3 lineEnd;

    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;

    int currentSelectedIndex;
    int currentSelectedType;
    int currentGizmoIndex;
    int currentGizmoType;

    int sizeH;
    int sizeW;

    void draw(bool val);
    void drawBranches(bool wire);
    void drawLiners();
    void computeBranchRadii();
    bool canSee(Point3*, Point3*);

    bool nodeToBranchOpt;
    bool nodeToCenterOpt;
    bool nodeInBlockOpt;
};
//! [3]

#endif
