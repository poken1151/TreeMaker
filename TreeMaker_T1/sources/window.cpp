/*
 * windows.cpp
 *
 * Author:
 *     Owen Patrick, Qt
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
#include "window.h"

//! [0]
Window::Window()
{
    glWidget = new GLWidget;
//    glWidget->setFixedHeight(600); //800
//    glWidget->setFixedWidth(900);  //1280
    glWidget->setMinimumHeight(600); //800
    glWidget->setMinimumWidth(900);  //1280

    //Widgets to hold the different layouts.
    QWidget *allContainer = new QWidget();
    QWidget *basicLayout = new QWidget();
    QWidget *blockLayout  = new QWidget();
    QWidget *cloudLayout = new QWidget();
    QWidget *drawLayout = new QWidget();

    //ageSlider = new QSlider();

    //Initializing the class wide Layouts.
    QHBoxLayout *mainLayout = new QHBoxLayout;
    scrollArea = new QScrollArea;
    allLayouts = new QStackedLayout;
    noSelectLayout = new QVBoxLayout;
    blockSelectLayout = new QVBoxLayout;
    cloudSelectLayout = new QVBoxLayout;
    drawMenuLayout = new QVBoxLayout;

    //In this section we are going to set up
    //all the specific widgets for each layout.
    ageSlider = createSlider();
        ageSlider->setToolTip("Tree Age");
    girthSlider = createSlider();
        girthSlider->setToolTip("Branch Weight");
    girthSlider->setValue(100);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    createBasicLayout();
    //noSelectLayout->addWidget(switchToCloud);

    createBlockLayout();

    createCloudLayout();

    createDrawMenu();

    //Setting all the layouts to their parent widgets.
    basicLayout->setLayout(noSelectLayout);
    blockLayout->setLayout(blockSelectLayout);
    cloudLayout->setLayout(cloudSelectLayout);
    drawLayout->setLayout(drawMenuLayout);
    scrollArea->setWidget(drawLayout);

//    basicLayout->setMinimumWidth(200);
//    blockLayout->setMinimumWidth(200);
//    cloudLayout->setMinimumWidth(200);
//    drawLayout->setMinimumWidth(200);
//    scrollArea->setMinimumWidth(200);
//    basicLayout->setMaximumWidth(230);
//    blockLayout->setMaximumWidth(230);
//    cloudLayout->setMaximumWidth(230);
//    drawLayout->setMaximumWidth(230);
//    scrollArea->setMaximumWidth(230);

    //Setting all the layout widgets to the stacked layout.
    //Order: 0 - basic, 1 - cloud, 2 - block
    allLayouts->addWidget(basicLayout);
    allLayouts->addWidget(cloudLayout);
    allLayouts->addWidget(blockLayout);
    allLayouts->addWidget(scrollArea);
    allContainer->setLayout(allLayouts);
    allContainer->setMaximumWidth(230);

    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ageSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    girthSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    allContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    //Defining the main Layout. Only two Items:
    //The GLWiget and the StackedLayout which is switchable.
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(ageSlider);
    mainLayout->addWidget(girthSlider);
    mainLayout->addWidget(allContainer);
    setLayout(mainLayout);

    allLayouts->setCurrentIndex(0);


    connect(glWidget, SIGNAL(ageLimitChanged(int,int)), this, SLOT(setTreeAgeSlider(int,int)));
    connect(ageSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setAgeLimit(int)));
    connect(girthSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(updateGirth(int)));
    connect(glWidget, SIGNAL(sceneSelectionChanged(int)), allLayouts, SLOT(setCurrentIndex(int)));

    setWindowTitle(tr("TreeMaker Trial"));
}
//! [0]

//! [1]
QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(1, 100);
    slider->setSingleStep(1);
    slider->setPageStep(10);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksLeft);
    return slider;
}
//! [1]

//! [2]
QSlider *Window::createSlider2()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    //slider->setRange(1, 100);
    slider->setSingleStep(1);
    slider->setPageStep(1);
    slider->setTickInterval(15);
    slider->setTickPosition(QSlider::TicksLeft);
    return slider;
}
//! [2]

QPushButton *Window::createButton()
{
    QPushButton *button = new QPushButton();
//    button->setFixedHeight(5);
//    button->setFixedWidth(15);
    return button;
}

void Window::switchLayout(bool tim)
{

}

void Window::setTreeAgeSlider(int base, int val)
{
    ageSlider->setRange(0, val);
    ageSlider->setValue(val);
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        //QWidget::keyPressEvent(e);
        glWidget->keyPressEvent(e);
}

void Window::createDrawMenu()
{
    QGroupBox *cloudDraw = new QGroupBox(tr("Cloud Draw Options"));
    QGroupBox *leafDraw = new QGroupBox(tr("Point Draw Options"));
    QGroupBox *boxDraw = new QGroupBox(tr("Box Draw Options"));
    QGroupBox *treeDraw = new QGroupBox(tr("Tree Draw Options"));
    QGroupBox *growOptions = new QGroupBox(tr("Grow Options"));

    QVBoxLayout *vboxCloud = new QVBoxLayout;
    QVBoxLayout *vboxLeaf = new QVBoxLayout;
    QVBoxLayout *vboxBox = new QVBoxLayout;
    QVBoxLayout *vboxTree = new QVBoxLayout;
    QVBoxLayout *vboxGrow = new QVBoxLayout;
    QVBoxLayout *graviLay = new QVBoxLayout;
    QVBoxLayout *ageLayA = new QVBoxLayout;
    QHBoxLayout *ageLayB = new QHBoxLayout;

    QLabel *graviLab = new QLabel("Gravitropism 1.0 - 10.0:");
    QLabel *growAgeLab = new QLabel("Age Attraction:");
    QSlider *growAgeSlide  = createSlider2();
    QSpinBox *growAgeSpin  = new QSpinBox;

    QPushButton *clearAllBranches= new QPushButton("Clear All Branches");
    QPushButton *drawAABB= new QPushButton("Draw Octree");
    QCheckBox *cloudDrawShell = new QCheckBox(tr("&Draw Cloud Shell"));
    QRadioButton *cloudShellSolid = new QRadioButton(tr("&Solid Shell"));
    QRadioButton *cloudSHellTrans = new QRadioButton(tr("&Transparent Shell"));

    QCheckBox *leafDrawNode = new QCheckBox(tr("&Draw Goal Nodes"));
    QCheckBox *leafDrawKillRad = new QCheckBox(tr("&Kill Radius"));
    QCheckBox *leafDrawInflRad = new QCheckBox(tr("&Influence Radius"));

    QCheckBox *boxDrawSelf = new QCheckBox(tr("&Draw Boxes"));
    QRadioButton *boxDrawSolid = new QRadioButton(tr("&Solid"));
    QRadioButton *boxDrawFrame = new QRadioButton(tr("&Wireframe"));

    QRadioButton *treeDrawFrame = new QRadioButton(tr("&Tree: Wireframe"));
    QRadioButton *treeDrawCylin = new QRadioButton(tr("&Tree: SolidCyl"));

    QRadioButton *growOnlyVisible = new QRadioButton(tr("&Only Visible"));
    QRadioButton *growBothVisible = new QRadioButton(tr("&Visible \& Invisible"));
    QCheckBox *nodeToBranch = new QCheckBox(tr("&Consider Node Visibility"));
    QCheckBox *nodeToCenter = new QCheckBox(tr("&Consider Center Visibility"));
    QCheckBox *nodeInBlock = new QCheckBox(tr("&Exclude Nodes IN blocks"));

    QDoubleSpinBox *graviSpinB = new QDoubleSpinBox();

    QPushButton *exportPython= new QPushButton("Export Python Script");
    QPushButton *toBasicLayout= new QPushButton("<-- Main Page");

    graviSpinB->setRange(0, 10);
    growAgeSpin->setRange(0,9999);
    growAgeSlide->setRange(0,9999);


    vboxCloud->addWidget(cloudDrawShell);
    vboxCloud->addWidget(cloudShellSolid);
    vboxCloud->addWidget(cloudSHellTrans);

//    vboxLeaf->addWidget(leafDrawNode);
//    vboxLeaf->addWidget(leafDrawKillRad);
//    vboxLeaf->addWidget(leafDrawInflRad);
    vboxCloud->addWidget(leafDrawNode);
    vboxCloud->addWidget(leafDrawKillRad);
    vboxCloud->addWidget(leafDrawInflRad);

    vboxBox->addWidget(boxDrawSelf);
    vboxBox->addWidget(boxDrawSolid);
    vboxBox->addWidget(boxDrawFrame);

    vboxTree->addWidget(treeDrawFrame);
    vboxTree->addWidget(treeDrawCylin);

    vboxGrow->addWidget(growOnlyVisible);
    vboxGrow->addWidget(growBothVisible);
    vboxGrow->addWidget(nodeToBranch);
    vboxGrow->addWidget(nodeToCenter);
    vboxGrow->addWidget(nodeInBlock);

    cloudDraw->setLayout(vboxCloud);
    leafDraw->setLayout(vboxLeaf);
    boxDraw->setLayout(vboxBox);
    treeDraw->setLayout(vboxTree);
    growOptions->setLayout(vboxGrow);
    graviLay->addWidget(graviLab);
    graviLay->addWidget(graviSpinB);
    ageLayB->addWidget(growAgeLab, 0, Qt::AlignLeft);
    ageLayB->addWidget(growAgeSpin, 0, Qt::AlignRight);
    ageLayA->addLayout(ageLayB);
    ageLayA->addWidget(growAgeSlide);


    connect(cloudDrawShell, SIGNAL(toggled(bool)), glWidget, SLOT(toggleDrawShell(bool)));
    connect(cloudShellSolid, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawSolid(bool)));
    connect(cloudSHellTrans, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawTrans(bool)));
    connect(leafDrawNode, SIGNAL(toggled(bool)), glWidget, SLOT(toggleDrawNodes(bool)));
    connect(leafDrawKillRad, SIGNAL(toggled(bool)), glWidget, SLOT(toggleDrawKillR(bool)));
    connect(leafDrawInflRad, SIGNAL(toggled(bool)), glWidget, SLOT(toggleDrawInflR(bool)));
    connect(boxDrawSelf, SIGNAL(toggled(bool)), glWidget, SLOT(toggleDrawBoxes(bool)));
    connect(boxDrawSolid, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawBoxSo(bool)));
    connect(boxDrawFrame, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawBoxWi(bool)));
    connect(treeDrawFrame, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawTreeF(bool)));
    connect(treeDrawCylin, SIGNAL(clicked(bool)), glWidget, SLOT(toggleDrawTreeS(bool)));
    connect(growOnlyVisible, SIGNAL(clicked(bool)), glWidget, SLOT(toggleGrowOptionWire(bool)));
    connect(growBothVisible, SIGNAL(clicked(bool)), glWidget, SLOT(toggleGrowOptionSolid(bool)));
    connect(nodeToBranch, SIGNAL(toggled(bool)), glWidget, SLOT(toggleNodeBranch(bool)));
    connect(nodeToCenter, SIGNAL(toggled(bool)), glWidget, SLOT(toggleNodeCenter(bool)));
    connect(nodeInBlock, SIGNAL(toggled(bool)), glWidget, SLOT(toggleNodeBlock(bool)));
    connect(clearAllBranches, SIGNAL(clicked(bool)), glWidget, SLOT(clearAllTreeBranches(bool)));
    connect(exportPython, SIGNAL(clicked(bool)), glWidget, SLOT(createPythonScript(bool)));
    connect(drawAABB, SIGNAL(clicked(bool)), glWidget, SLOT(toggleAABB(bool)));
    connect(toBasicLayout, SIGNAL(clicked(bool)), glWidget, SLOT(switchToMainOptions(bool)));

    connect(boxDrawSelf, SIGNAL(toggled(bool)), boxDrawSolid, SLOT(setEnabled(bool)));
    connect(boxDrawSelf, SIGNAL(toggled(bool)), boxDrawFrame, SLOT(setEnabled(bool)));

    connect(cloudDrawShell, SIGNAL(toggled(bool)), cloudShellSolid, SLOT(setEnabled(bool)));
    connect(cloudDrawShell, SIGNAL(toggled(bool)), cloudSHellTrans, SLOT(setEnabled(bool)));

    //connect(glWidget, SIGNAL(ageLimitChanged(int,int)), growAgeSlide, SLOT(setRange(int,int)));
    //connect(glWidget, SIGNAL(growLimitChanged(int)), growAgeSlide, SLOT(setValue(int))); it will change every iteration, so not sure I want that...
    connect(growAgeSpin, SIGNAL(valueChanged(int)), glWidget, SLOT(setGrowLimit(int)));
    connect(growAgeSpin, SIGNAL(valueChanged(int)), growAgeSlide, SLOT(setValue(int)));
    connect(growAgeSlide, SIGNAL(valueChanged(int)), growAgeSpin, SLOT(setValue(int)));


    connect(graviSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setGraviTrop(double)));
    graviSpinB->setValue(0.0);
    growAgeSpin->setValue(9999);


    cloudDrawShell->setChecked(true);
    cloudDrawShell->setChecked(false);
    cloudShellSolid->setChecked(false);
    cloudSHellTrans->setChecked(true);
    leafDrawNode->setChecked(true);
    leafDrawKillRad->setChecked(false);
    leafDrawInflRad->setChecked(false);
    boxDrawSelf->setChecked(true);
    boxDrawSolid->setChecked(true);
    boxDrawFrame->setChecked(false);
    treeDrawFrame->setChecked(false);
    treeDrawCylin->setChecked(true);
    growBothVisible->setChecked(false);
    growOnlyVisible->setChecked(true);


    drawMenuLayout->addWidget(clearAllBranches);
    drawMenuLayout->addWidget(cloudDraw);
    //drawMenuLayout->addWidget(leafDraw);
    drawMenuLayout->addWidget(boxDraw);
    drawMenuLayout->addWidget(treeDraw);
    drawMenuLayout->addWidget(growOptions);
    drawMenuLayout->addLayout(graviLay);
    drawMenuLayout->addLayout(ageLayA);
    drawMenuLayout->addWidget(drawAABB);
    drawMenuLayout->addWidget(exportPython);
    drawMenuLayout->addWidget(toBasicLayout);
}

void Window::createBasicLayout()
{
//    //Initializing the class wide Layouts.
//    QHBoxLayout *mainLayout = new QHBoxLayout;

    QLabel *colLable = new QLabel("Col Value"); colLable->setFont(QFont("Arial", 8));
    QLabel *cntLable = new QLabel("# of Nodes"); colLable->setFont(QFont("Arial", 8));
    QLabel *shpLable = new QLabel("Cloud Shape"); shpLable->setFont(QFont("Arial", 8));
    QLabel *colLable2 = new QLabel(); colLable->setFont(QFont("Arial", 8));
    QLabel *cntLable2 = new QLabel("# of Nodes"); colLable->setFont(QFont("Arial", 8));
    QLabel *shpLable2 = new QLabel("Cloud Shape"); shpLable->setFont(QFont("Arial", 8));
    QLabel *infLable = new QLabel("Influence Dist.");
    QLabel *kilLable = new QLabel("Kill Dist.");
    QLabel *xRaLable = new QLabel("X-Radius"); xRaLable->setFont(QFont("Arial", 8));
    QLabel *yRaLable = new QLabel("Y-Radius"); yRaLable->setFont(QFont("Arial", 8));
    QLabel *zRaLable = new QLabel("Z-Radius"); zRaLable->setFont(QFont("Arial", 8));
    QLabel *cSzLable = new QLabel("Cloud \"Size\""); cSzLable->setFont(QFont("Arial", 8));
    QLabel *sedLable = new QLabel("Rndm Seed"); sedLable->setFont(QFont("Arial", 8));
    QLabel *xRaLable2 = new QLabel(); xRaLable->setFont(QFont("Arial", 8));
    QLabel *yRaLable2 = new QLabel(); yRaLable->setFont(QFont("Arial", 8));
    QLabel *zRaLable2 = new QLabel(); zRaLable->setFont(QFont("Arial", 8));
    QLabel *cSzLable2 = new QLabel(); cSzLable->setFont(QFont("Arial", 8));
    QLabel *sedLable2 = new QLabel(); sedLable->setFont(QFont("Arial", 8));

    QLabel *trnkLable = new QLabel("Initialize Trunk:");
    QLabel *xPosLable = new QLabel("X Pos.");
    QLabel *yPosLable = new QLabel("Y Pos.");
    QLabel *zPosLable = new QLabel("Z Pos.");
    QLabel *hiteLable = new QLabel("Trunk Height"); hiteLable->setFont(QFont("Arial", 8));
    QLabel *hiteLable2 = new QLabel(); hiteLable->setFont(QFont("Arial", 8));

    QLabel *blokLable = new QLabel("Create Block");
    QLabel *lentLable = new QLabel("Length");
    QLabel *widtLable = new QLabel("Width");
    QLabel *heitLable = new QLabel("Height");
    QLabel *axisLable = new QLabel("Main Axis");

    QLabel *stepLable = new QLabel("Step Size"); stepLable->setFont(QFont("Arial", 8));
    QLabel *stepLable2 = new QLabel(); stepLable->setFont(QFont("Arial", 8));


    QSlider *colSpinB = createSlider2();
    QSpinBox *cntSpinB = new QSpinBox;
    QComboBox *shpSpinB = new QComboBox;
        shpSpinB->addItem("Spherical");
        shpSpinB->addItem("Conical");
        shpSpinB->addItem("Elliptical");
        shpSpinB->addItem("Cube");
        shpSpinB->addItem("Hemi-Ellipse");
        shpSpinB->addItem("Reverse Hemi");
        shpSpinB->addItem("Reverse Cone");
    QSlider *infSpinB = createSlider2();
    QSlider *kilSpinB = createSlider2();
    QSlider *xRaSpinB = createSlider2();
    QSlider *yRaSpinB = createSlider2();
    QSlider *zRaSpinB = createSlider2();
    QSlider *cSzSpinB = createSlider2();
    QSpinBox *sedSpinB = new QSpinBox;
    QPushButton *createCloudButton = new QPushButton("Add New Leaf Cloud");
    QPushButton *displayOptions = new QPushButton("Options/Export -->");

    QSlider *xPosSpinB = createSlider2();
    QSlider *yPosSpinB = createSlider2();
    QSlider *zPosSpinB = createSlider2();
    QSlider *hiteSpinB = createSlider2();
    QPushButton *setTrunkButton = new QPushButton("Create Trunk");

    QSlider *lentSpinB = createSlider2();
    QSlider *widtSpinB = createSlider2();
    QSlider *heitSpinB = createSlider2();
    QComboBox *axisSpinB = new QComboBox;
        axisSpinB->addItem("X-Axis");
        axisSpinB->addItem("Z-Axis");
        QPushButton *createBlockButton = new QPushButton("Create Block");
        QPushButton *createEllipseButton = new QPushButton("Create Ellipsoid");

    QSlider *stepSpinB = createSlider2();
    QPushButton *growTreeButton = new QPushButton("Grow Tree");


    colSpinB->setRange(0,255);
    cntSpinB->setRange(0,10000);
    infSpinB->setRange(100,50000);
    kilSpinB->setRange(100,50000);
    xRaSpinB->setRange(100,10000);
    yRaSpinB->setRange(100,10000);
    zRaSpinB->setRange(100,10000);
    xRaSpinB->setTickInterval(1000);
    yRaSpinB->setTickInterval(1000);
    zRaSpinB->setTickInterval(1000);
    cSzSpinB->setRange(100,50000);
    sedSpinB->setRange(1,99999999);


    xPosSpinB->setRange(-50000,50000);
    yPosSpinB->setRange(-25000,50000);
    zPosSpinB->setRange(-50000,50000);
    hiteSpinB->setRange(1,500);

    lentSpinB->setRange(100,100000);
    widtSpinB->setRange(100,100000);
    heitSpinB->setRange(100,100000);

    stepSpinB->setRange(1,10000);


    connect(colSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setColValue(int)));
    connect(colSpinB, SIGNAL(valueChanged(int)), colLable2, SLOT(setNum(int)));
    connect(cntSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCntValue(int)));
    connect(shpSpinB, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setShpValue(int)));
    connect(infSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setInfValue(int)));
    connect(kilSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setKilValue(int)));
    connect(xRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRaValue(int)));
    connect(yRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRaValue(int)));
    connect(zRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRaValue(int)));
    connect(xRaSpinB, SIGNAL(valueChanged(int)), xRaLable2, SLOT(setNum(int)));
    connect(yRaSpinB, SIGNAL(valueChanged(int)), yRaLable2, SLOT(setNum(int)));
    connect(zRaSpinB, SIGNAL(valueChanged(int)), zRaLable2, SLOT(setNum(int)));
    connect(cSzSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCSzValue(int)));
    connect(cSzSpinB, SIGNAL(valueChanged(int)), cSzLable2, SLOT(setNum(int)));
    connect(sedSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setSedValue(int)));
    connect(createCloudButton, SIGNAL(clicked(bool)), glWidget, SLOT(createNewCloud(bool)));

    connect(hiteSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setTrunkHeight(int)));
    connect(hiteSpinB, SIGNAL(valueChanged(int)), hiteLable2, SLOT(setNum(int)));
    connect(setTrunkButton, SIGNAL(clicked(bool)), glWidget, SLOT(setTrunk(bool)));

    connect(lentSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setBoxLen(int)));
    connect(widtSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setBoxWid(int)));
    connect(heitSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setBoxHgt(int)));
    connect(axisSpinB, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setBoxAxis(int)));
    connect(createBlockButton, SIGNAL(clicked(bool)), glWidget, SLOT(addBlock(bool)));
    connect(createEllipseButton, SIGNAL(clicked(bool)), glWidget, SLOT(addEllipsoid(bool)));

    connect(stepSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setStepSize(int)));
    connect(stepSpinB, SIGNAL(valueChanged(int)), stepLable2, SLOT(setNum(int)));
    connect(growTreeButton, SIGNAL(clicked(bool)), glWidget, SLOT(growTreeOriginal(bool)));
    connect(displayOptions, SIGNAL(clicked(bool)), glWidget, SLOT(switchToDrawOptions(bool)));


    colSpinB->setValue(100);
    cntSpinB->setValue(3000);
    shpSpinB->setCurrentIndex(2);
    infSpinB->setValue(4500);
    kilSpinB->setValue(1500);
    xRaSpinB->setValue(6500);
    yRaSpinB->setValue(6500);
    zRaSpinB->setValue(6500);
    cSzSpinB->setValue(20000);
    sedSpinB->setValue(2375836);

    hiteSpinB->setValue(45);

    lentSpinB->setValue(5000);
    widtSpinB->setValue(1000);
    heitSpinB->setValue(2000);
    axisSpinB->setCurrentIndex(1);

    stepSpinB->setValue(3000);


    QHBoxLayout *colBoxL = new QHBoxLayout;
    QHBoxLayout *colBoxL2 = new QHBoxLayout;
    QHBoxLayout *cntBoxL = new QHBoxLayout;
    QHBoxLayout *shpBoxL = new QHBoxLayout;
    QHBoxLayout *infBoxL = new QHBoxLayout;
    QHBoxLayout *kilBoxL = new QHBoxLayout;
    QHBoxLayout *xRaBoxL = new QHBoxLayout;
    QHBoxLayout *yRaBoxL = new QHBoxLayout;
    QHBoxLayout *zRaBoxL = new QHBoxLayout;
    QHBoxLayout *cSzBoxL = new QHBoxLayout;
    QHBoxLayout *xRaBoxL2 = new QHBoxLayout;
    QHBoxLayout *yRaBoxL2 = new QHBoxLayout;
    QHBoxLayout *zRaBoxL2 = new QHBoxLayout;
    QHBoxLayout *cSzBoxL2 = new QHBoxLayout;
    QHBoxLayout *sedBoxL = new QHBoxLayout;

    QHBoxLayout *trnkBoxL = new QHBoxLayout;
    QHBoxLayout *xPosBoxL = new QHBoxLayout;
    QHBoxLayout *yPosBoxL = new QHBoxLayout;
    QHBoxLayout *zPosBoxL = new QHBoxLayout;
    QHBoxLayout *hiteBoxL = new QHBoxLayout;
    QHBoxLayout *hiteBoxL2 = new QHBoxLayout;

    QHBoxLayout *blokBoxL = new QHBoxLayout;
    QHBoxLayout *lentBoxL = new QHBoxLayout;
    QHBoxLayout *widtBoxL = new QHBoxLayout;
    QHBoxLayout *heitBoxL = new QHBoxLayout;
    QHBoxLayout *axisBoxL = new QHBoxLayout;

    QHBoxLayout *stepBoxL = new QHBoxLayout;
    QHBoxLayout *stepBoxL2 = new QHBoxLayout;


    colBoxL->addWidget(colLable, Qt::AlignLeft);
    colBoxL->addWidget(colLable2, Qt::AlignLeft);
    cntBoxL->addWidget(cntLable, Qt::AlignLeft);
    shpBoxL->addWidget(shpLable, Qt::AlignLeft);
    xRaBoxL->addWidget(xRaLable, Qt::AlignLeft);
    yRaBoxL->addWidget(yRaLable, Qt::AlignLeft);
    zRaBoxL->addWidget(zRaLable, Qt::AlignLeft);
    xRaBoxL->addWidget(xRaLable2, Qt::AlignLeft);
    yRaBoxL->addWidget(yRaLable2, Qt::AlignLeft);
    zRaBoxL->addWidget(zRaLable2, Qt::AlignLeft);
    cSzBoxL->addWidget(cSzLable, Qt::AlignLeft);
    cSzBoxL->addWidget(cSzLable2, Qt::AlignLeft);
    sedBoxL->addWidget(sedLable, Qt::AlignLeft);

    hiteBoxL->addWidget(hiteLable, Qt::AlignLeft);
    hiteBoxL->addWidget(hiteLable2, Qt::AlignLeft);

    stepBoxL->addWidget(stepLable, Qt::AlignLeft);
    stepBoxL->addWidget(stepLable2, Qt::AlignLeft);


    colBoxL2->addWidget(colSpinB, Qt::AlignRight);
    cntBoxL->addWidget(cntSpinB, Qt::AlignRight);
    shpBoxL->addWidget(shpSpinB, Qt::AlignRight);
    xRaBoxL2->addWidget(xRaSpinB, Qt::AlignRight);
    yRaBoxL2->addWidget(yRaSpinB, Qt::AlignRight);
    zRaBoxL2->addWidget(zRaSpinB, Qt::AlignRight);
    cSzBoxL2->addWidget(cSzSpinB, Qt::AlignRight);
    sedBoxL->addWidget(sedSpinB, Qt::AlignRight);

    hiteBoxL2->addWidget(hiteSpinB, Qt::AlignRight);

    stepBoxL2->addWidget(stepSpinB, Qt::AlignRight);


    noSelectLayout->addLayout(colBoxL);
    noSelectLayout->addLayout(colBoxL2);
    noSelectLayout->addLayout(cntBoxL);
    noSelectLayout->addLayout(shpBoxL);
    noSelectLayout->addLayout(infBoxL);
    noSelectLayout->addLayout(kilBoxL);
    noSelectLayout->addLayout(xRaBoxL);
    noSelectLayout->addLayout(xRaBoxL2);
    noSelectLayout->addLayout(yRaBoxL);
    noSelectLayout->addLayout(yRaBoxL2);
    noSelectLayout->addLayout(zRaBoxL);
    noSelectLayout->addLayout(zRaBoxL2);
//    noSelectLayout->addLayout(cSzBoxL);
//    noSelectLayout->addLayout(cSzBoxL2);
    noSelectLayout->addLayout(sedBoxL);
    noSelectLayout->addWidget(createCloudButton);
    noSelectLayout->addLayout(trnkBoxL);
    noSelectLayout->addLayout(xPosBoxL);
    noSelectLayout->addLayout(yPosBoxL);
    noSelectLayout->addLayout(zPosBoxL);
    noSelectLayout->addLayout(hiteBoxL);
    noSelectLayout->addLayout(hiteBoxL2);
    noSelectLayout->addWidget(setTrunkButton);
    noSelectLayout->addLayout(blokBoxL);
    noSelectLayout->addLayout(lentBoxL);
    noSelectLayout->addLayout(widtBoxL);
    noSelectLayout->addLayout(heitBoxL);
    noSelectLayout->addLayout(axisBoxL);
    noSelectLayout->addWidget(createBlockButton);
    noSelectLayout->addWidget(createEllipseButton);
    noSelectLayout->addLayout(stepBoxL);
    noSelectLayout->addLayout(stepBoxL2);
    noSelectLayout->addWidget(growTreeButton);
    noSelectLayout->addWidget(displayOptions);
    noSelectLayout->addStretch(0);
}

void Window::createCloudLayout()
{
    //    //Initializing the class wide Layouts.
    //    QHBoxLayout *mainLayout = new QHBoxLayout;

        QLabel *xPosLable = new QLabel("X Pos.");
        QLabel *yPosLable = new QLabel("Y Pos.");
        QLabel *zPosLable = new QLabel("Z Pos.");

        QLabel *cldShapeLabel = new QLabel("Cloud Shape");
        QLabel *xRaLable = new QLabel("X Radius");
        QLabel *yRaLable = new QLabel("Y Radius");
        QLabel *zRaLable = new QLabel("Z Radius");
        QLabel *cldSizeLable = new QLabel("Cloud Size");

        QLabel *numNodesLable = new QLabel("No. of Points");
        QLabel *killLable = new QLabel("Kill Radius");
        QLabel *inflLable = new QLabel("Influence Rad.");
        //QLabel *Lable = new QLabel("Z Pos.");


        QSpinBox *xPosSpinB = new QSpinBox;
        QSpinBox *yPosSpinB = new QSpinBox;
        QSpinBox *zPosSpinB = new QSpinBox;

        QSlider *xPosSlide = createSlider2();
        QSlider *yPosSlide = createSlider2();
        QSlider *zPosSlide = createSlider2();

        QComboBox *cloudShape = new QComboBox;
            cloudShape->addItem("Spherical");
            cloudShape->addItem("Conical");
            cloudShape->addItem("Elliptical");
            cloudShape->addItem("Cube");
            cloudShape->addItem("Hemi-Ellipse");
            cloudShape->addItem("Reverse Hemi");
            cloudShape->addItem("Reverse Cone");


        QSpinBox *xRaSpinB = new QSpinBox;
        QSpinBox *yRaSpinB = new QSpinBox;
        QSpinBox *zRaSpinB = new QSpinBox;
        QSpinBox *cldSizeSpinB = new QSpinBox;

        QSpinBox *numNodesSpinB = new QSpinBox;
        QSpinBox *killSpinB = new QSpinBox;
        QSpinBox *inflSpinB = new QSpinBox;
        //QSpinBox *zPosSpinB = new QSpinBox;

        QSlider *xRaSlide = createSlider2();
        QSlider *yRaSlide = createSlider2();
        QSlider *zRaSlide = createSlider2();
        QSlider *cldSizeSlide = createSlider2();

        QSlider *numOfNodesSlide = createSlider2();
        QSlider *killSlide = createSlider2();
        QSlider *inflSlide = createSlider2();

        QPushButton *deleteCloudButton = new QPushButton("Delete Selected Cloud");
        QPushButton *toBasicLayout= new QPushButton("Deselect");

        xPosSpinB->setRange(-50000,50000);
        yPosSpinB->setRange(-25000,50000);
        zPosSpinB->setRange(-50000,50000);

        xPosSlide->setRange(-50000,50000);
        yPosSlide->setRange(-25000,50000);
        zPosSlide->setRange(-50000,50000);


        xRaSpinB->setRange(100,10000);
        yRaSpinB->setRange(100,10000);
        zRaSpinB->setRange(100,10000);
        cldSizeSpinB->setRange(100,100000);

        numNodesSpinB->setRange(1,10000);
        killSpinB->setRange(100,2500);
        inflSpinB->setRange(2500,20000);

        xRaSlide->setRange(100,10000);
        yRaSlide->setRange(100,10000);
        zRaSlide->setRange(100,10000);
        xRaSlide->setTickInterval(1000);
        yRaSlide->setTickInterval(1000);
        zRaSlide->setTickInterval(1000);
        cldSizeSlide->setRange(100,100000);

        numOfNodesSlide->setRange(1,10000);
        killSlide->setRange(100,2500);
        inflSlide->setRange(2500,20000);

        connect(xPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setXPos(int)));
        connect(yPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setYPos(int)));
        connect(zPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setZPos(int)));
        connect(xPosSpinB, SIGNAL(valueChanged(int)), xPosSlide, SLOT(setValue(int)));
        connect(yPosSpinB, SIGNAL(valueChanged(int)), yPosSlide, SLOT(setValue(int)));
        connect(zPosSpinB, SIGNAL(valueChanged(int)), zPosSlide, SLOT(setValue(int)));

//        connect(xPosSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setXPos(int)));
//        connect(yPosSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setYPos(int)));
//        connect(zPosSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setZPos(int)));
        connect(xPosSlide, SIGNAL(valueChanged(int)), xPosSpinB, SLOT(setValue(int)));
        connect(yPosSlide, SIGNAL(valueChanged(int)), yPosSpinB, SLOT(setValue(int)));
        connect(zPosSlide, SIGNAL(valueChanged(int)), zPosSpinB, SLOT(setValue(int)));

        connect(glWidget, SIGNAL(xPosChanged(int)), xPosSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(yPosChanged(int)), yPosSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(zPosChanged(int)), zPosSpinB, SLOT(setValue(int)));

        connect(xRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudXRa(int)));
        connect(yRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudYRa(int)));
        connect(zRaSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudZRa(int)));
        connect(cldSizeSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudSiz(int)));
        connect(xRaSpinB, SIGNAL(valueChanged(int)), xRaSlide, SLOT(setValue(int)));
        connect(yRaSpinB, SIGNAL(valueChanged(int)), yRaSlide, SLOT(setValue(int)));
        connect(zRaSpinB, SIGNAL(valueChanged(int)), zRaSlide, SLOT(setValue(int)));
        connect(cldSizeSpinB, SIGNAL(valueChanged(int)), cldSizeSlide, SLOT(setValue(int)));

        connect(numNodesSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudNum(int)));
        connect(killSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudKil(int)));
        connect(inflSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudInf(int)));
        connect(numNodesSpinB, SIGNAL(valueChanged(int)), numOfNodesSlide, SLOT(setValue(int)));
        connect(killSpinB, SIGNAL(valueChanged(int)), killSlide, SLOT(setValue(int)));
        connect(inflSpinB, SIGNAL(valueChanged(int)), inflSlide, SLOT(setValue(int)));

        connect(glWidget, SIGNAL(cloudXRaChanged(int)), xRaSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(cloudYRaChanged(int)), yRaSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(cloudZRaChanged(int)), zRaSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(cloudSizChanged(int)), cldSizeSpinB, SLOT(setValue(int)));

        connect(glWidget, SIGNAL(cloudNumChanged(int)), numNodesSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(cloudKilChanged(int)), killSpinB, SLOT(setValue(int)));
        connect(glWidget, SIGNAL(cloudInfChanged(int)), inflSpinB, SLOT(setValue(int)));

//        connect(xRaSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudXRa(int)));
//        connect(yRaSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudYRa(int)));
//        connect(zRaSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudZRa(int)));
//        connect(cldSizeSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudSiz(int)));
        connect(xRaSlide, SIGNAL(valueChanged(int)), xRaSpinB, SLOT(setValue(int)));
        connect(yRaSlide, SIGNAL(valueChanged(int)), yRaSpinB, SLOT(setValue(int)));
        connect(zRaSlide, SIGNAL(valueChanged(int)), zRaSpinB, SLOT(setValue(int)));
        connect(cldSizeSlide, SIGNAL(valueChanged(int)), cldSizeSpinB, SLOT(setValue(int)));

//        connect(numOfNodesSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudNum(int)));
//        connect(killSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudKil(int)));
//        connect(inflSlide, SIGNAL(valueChanged(int)), glWidget, SLOT(setCloudInf(int)));
        connect(numOfNodesSlide, SIGNAL(valueChanged(int)), numNodesSpinB, SLOT(setValue(int)));
        connect(killSlide, SIGNAL(valueChanged(int)), killSpinB, SLOT(setValue(int)));
        connect(inflSlide, SIGNAL(valueChanged(int)), inflSpinB, SLOT(setValue(int)));


        connect(cloudShape, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setCloudShape(int)));
        connect(glWidget, SIGNAL(cloudShpChanged(int)), cloudShape, SLOT(setCurrentIndex(int)));

        connect(deleteCloudButton, SIGNAL(clicked(bool)), glWidget, SLOT(deleteObject(bool)));        

        connect(toBasicLayout, SIGNAL(clicked(bool)), glWidget, SLOT(switchToMainOptions(bool)));

        QHBoxLayout *xPosBoxL = new QHBoxLayout;
        QHBoxLayout *yPosBoxL = new QHBoxLayout;
        QHBoxLayout *zPosBoxL = new QHBoxLayout;
        QHBoxLayout *cldShpeL = new QHBoxLayout;

        QHBoxLayout *xRaBoxL = new QHBoxLayout;
        QHBoxLayout *yRaBoxL = new QHBoxLayout;
        QHBoxLayout *zRaBoxL = new QHBoxLayout;
        QHBoxLayout *cldSizeL = new QHBoxLayout;

        QHBoxLayout *cldNumL = new QHBoxLayout;
        QHBoxLayout *cldKilL = new QHBoxLayout;
        QHBoxLayout *cldInfL = new QHBoxLayout;

        xPosBoxL->addWidget(xPosLable, Qt::AlignLeft);
        yPosBoxL->addWidget(yPosLable, Qt::AlignLeft);
        zPosBoxL->addWidget(zPosLable, Qt::AlignLeft);

        xPosBoxL->addWidget(xPosSpinB, Qt::AlignRight);
        yPosBoxL->addWidget(yPosSpinB, Qt::AlignRight);
        zPosBoxL->addWidget(zPosSpinB, Qt::AlignRight);


        xRaBoxL->addWidget(xRaLable, Qt::AlignLeft);
        yRaBoxL->addWidget(yRaLable, Qt::AlignLeft);
        zRaBoxL->addWidget(zRaLable, Qt::AlignLeft);
        cldSizeL->addWidget(cldSizeLable, Qt::AlignLeft);

        cldNumL->addWidget(numNodesLable, Qt::AlignLeft);
        cldKilL->addWidget(killLable, Qt::AlignLeft);
        cldInfL->addWidget(inflLable, Qt::AlignLeft);

        xRaBoxL->addWidget(xRaSpinB, Qt::AlignRight);
        yRaBoxL->addWidget(yRaSpinB, Qt::AlignRight);
        zRaBoxL->addWidget(zRaSpinB, Qt::AlignRight);
        cldSizeL->addWidget(cldSizeSpinB, Qt::AlignRight);

        cldNumL->addWidget(numNodesSpinB, Qt::AlignRight);
        cldKilL->addWidget(killSpinB, Qt::AlignRight);
        cldInfL->addWidget(inflSpinB, Qt::AlignRight);

        cldShpeL->addWidget(cldShapeLabel, Qt::AlignLeft);
        cldShpeL->addWidget(cloudShape, Qt::AlignRight);

        cloudSelectLayout->addLayout(xPosBoxL);
        cloudSelectLayout->addWidget(xPosSlide);
        cloudSelectLayout->addLayout(yPosBoxL);
        cloudSelectLayout->addWidget(yPosSlide);
        cloudSelectLayout->addLayout(zPosBoxL);
        cloudSelectLayout->addWidget(zPosSlide);
//        cloudSelectLayout->addLayout(cldSizeL);
//        cloudSelectLayout->addWidget(cldSizeSlide);
        cloudSelectLayout->addLayout(cldShpeL);
        cloudSelectLayout->addLayout(xRaBoxL);
        cloudSelectLayout->addWidget(xRaSlide);
        cloudSelectLayout->addLayout(yRaBoxL);
        cloudSelectLayout->addWidget(yRaSlide);
        cloudSelectLayout->addLayout(zRaBoxL);
        cloudSelectLayout->addWidget(zRaSlide);
        cloudSelectLayout->addLayout(cldNumL);
        cloudSelectLayout->addWidget(numOfNodesSlide);
        cloudSelectLayout->addLayout(cldKilL);
        cloudSelectLayout->addWidget(killSlide);
        cloudSelectLayout->addLayout(cldInfL);
        cloudSelectLayout->addWidget(inflSlide);
        cloudSelectLayout->addWidget(deleteCloudButton);
        cloudSelectLayout->addWidget(toBasicLayout);
        cloudSelectLayout->addStretch(0);

}

void Window::createBlockLayout()
{
    QLabel *xPosLable = new QLabel("X Pos.");
    QLabel *yPosLable = new QLabel("Y Pos.");
    QLabel *zPosLable = new QLabel("Z Pos.");    
    QLabel *lenSizeLable = new QLabel("Length:");
    QLabel *widSizeLable = new QLabel("Width:");
    QLabel *hgtSizeLable = new QLabel("Height:");
    QLabel *axiSizeLable = new QLabel("Allign to:");

    QSpinBox *xPosSpinB = new QSpinBox;
    QSpinBox *yPosSpinB = new QSpinBox;
    QSpinBox *zPosSpinB = new QSpinBox;    

    QSlider *xPosSlide = createSlider2();
    QSlider *yPosSlide = createSlider2();
    QSlider *zPosSlide = createSlider2();


    QSpinBox *lenSizeSpinB = new QSpinBox;
    QSpinBox *widSizeSpinB = new QSpinBox;
    QSpinBox *hgtSizeSpinB = new QSpinBox;

    QSlider *lenSizeSlide = createSlider2();
    QSlider *widSizeSlide = createSlider2();
    QSlider *hgtSizeSlide = createSlider2();


    QComboBox *axiSizeSpinB = new QComboBox;
        axiSizeSpinB->addItem("X-Axis");
        axiSizeSpinB->addItem("Z-Axis");

    QPushButton *deleteBoxButton = new QPushButton("Delete Selected Block");
    QPushButton *toBasicLayout= new QPushButton("Deselect Box");

    xPosSpinB->setRange(-50000,50000);
    yPosSpinB->setRange(-25000,50000);
    zPosSpinB->setRange(-50000,50000);

    xPosSlide->setRange(-50000,50000);
    yPosSlide->setRange(-25000,50000);
    zPosSlide->setRange(-50000,50000);


    lenSizeSpinB->setRange(50,20000);
    widSizeSpinB->setRange(50,20000);
    hgtSizeSpinB->setRange(50,20000);

    lenSizeSlide->setRange(50,20000);
    widSizeSlide->setRange(50,20000);
    hgtSizeSlide->setRange(50,20000);


    connect(glWidget, SIGNAL(xPosChanged(int)), xPosSpinB, SLOT(setValue(int)));
    connect(glWidget, SIGNAL(yPosChanged(int)), yPosSpinB, SLOT(setValue(int)));
    connect(glWidget, SIGNAL(zPosChanged(int)), zPosSpinB, SLOT(setValue(int)));
    connect(xPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setXPos(int)));
    connect(yPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setYPos(int)));
    connect(zPosSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setZPos(int)));
    connect(xPosSpinB, SIGNAL(valueChanged(int)), xPosSlide, SLOT(setValue(int)));
    connect(yPosSpinB, SIGNAL(valueChanged(int)), yPosSlide, SLOT(setValue(int)));
    connect(zPosSpinB, SIGNAL(valueChanged(int)), zPosSlide, SLOT(setValue(int)));
    connect(xPosSlide, SIGNAL(valueChanged(int)), xPosSpinB, SLOT(setValue(int)));
    connect(yPosSlide, SIGNAL(valueChanged(int)), yPosSpinB, SLOT(setValue(int)));
    connect(zPosSlide, SIGNAL(valueChanged(int)), zPosSpinB, SLOT(setValue(int)));

    connect(glWidget, SIGNAL(lenSizeChanged(int)), lenSizeSpinB, SLOT(setValue(int)));
    connect(glWidget, SIGNAL(widSizeChanged(int)), widSizeSpinB, SLOT(setValue(int)));
    connect(glWidget, SIGNAL(hgtSizeChanged(int)), hgtSizeSpinB, SLOT(setValue(int)));
    connect(lenSizeSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setSizeLen(int)));
    connect(widSizeSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setSizeWid(int)));
    connect(hgtSizeSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setSizeHgt(int)));
    connect(lenSizeSpinB, SIGNAL(valueChanged(int)), lenSizeSlide, SLOT(setValue(int)));
    connect(widSizeSpinB, SIGNAL(valueChanged(int)), widSizeSlide, SLOT(setValue(int)));
    connect(hgtSizeSpinB, SIGNAL(valueChanged(int)), hgtSizeSlide, SLOT(setValue(int)));
    connect(lenSizeSlide, SIGNAL(valueChanged(int)), lenSizeSpinB, SLOT(setValue(int)));
    connect(widSizeSlide, SIGNAL(valueChanged(int)), widSizeSpinB, SLOT(setValue(int)));
    connect(hgtSizeSlide, SIGNAL(valueChanged(int)), hgtSizeSpinB, SLOT(setValue(int)));

    connect(axiSizeSpinB, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setSizeAxi(int)));
    connect(glWidget, SIGNAL(axiSizeChanged(int)), axiSizeSpinB, SLOT(setCurrentIndex(int)));

    connect(deleteBoxButton, SIGNAL(clicked(bool)), glWidget, SLOT(deleteObject(bool)));
    connect(toBasicLayout, SIGNAL(clicked(bool)), glWidget, SLOT(switchToMainOptions(bool)));


    QHBoxLayout *xPosBoxL = new QHBoxLayout;
    QHBoxLayout *yPosBoxL = new QHBoxLayout;
    QHBoxLayout *zPosBoxL = new QHBoxLayout;
    QHBoxLayout *lenSizeBoxL = new QHBoxLayout;
    QHBoxLayout *widSizeBoxL = new QHBoxLayout;
    QHBoxLayout *hgtSizeBoxL = new QHBoxLayout;
    QHBoxLayout *axiSizeBoxL = new QHBoxLayout;

    xPosBoxL->addWidget(xPosLable, Qt::AlignLeft);
    yPosBoxL->addWidget(yPosLable, Qt::AlignLeft);
    zPosBoxL->addWidget(zPosLable, Qt::AlignLeft);
    lenSizeBoxL->addWidget(lenSizeLable, Qt::AlignLeft);
    widSizeBoxL->addWidget(widSizeLable, Qt::AlignLeft);
    hgtSizeBoxL->addWidget(hgtSizeLable, Qt::AlignLeft);
    axiSizeBoxL->addWidget(axiSizeLable, Qt::AlignLeft);

    xPosBoxL->addWidget(xPosSpinB, Qt::AlignRight);
    yPosBoxL->addWidget(yPosSpinB, Qt::AlignRight);
    zPosBoxL->addWidget(zPosSpinB, Qt::AlignRight);
    lenSizeBoxL->addWidget(lenSizeSpinB, Qt::AlignRight);
    widSizeBoxL->addWidget(widSizeSpinB, Qt::AlignRight);
    hgtSizeBoxL->addWidget(hgtSizeSpinB, Qt::AlignRight);
    axiSizeBoxL->addWidget(axiSizeSpinB, Qt::AlignRight);

    blockSelectLayout->addLayout(xPosBoxL);
    blockSelectLayout->addWidget(xPosSlide);
    blockSelectLayout->addLayout(yPosBoxL);
    blockSelectLayout->addWidget(yPosSlide);
    blockSelectLayout->addLayout(zPosBoxL);
    blockSelectLayout->addWidget(zPosSlide);
    blockSelectLayout->addLayout(lenSizeBoxL);
    blockSelectLayout->addWidget(lenSizeSlide);
    blockSelectLayout->addLayout(widSizeBoxL);
    blockSelectLayout->addWidget(widSizeSlide);
    blockSelectLayout->addLayout(hgtSizeBoxL);
    blockSelectLayout->addWidget(hgtSizeSlide);
    blockSelectLayout->addLayout(axiSizeBoxL);
    blockSelectLayout->addWidget(deleteBoxButton);
    blockSelectLayout->addWidget(toBasicLayout);
    blockSelectLayout->addStretch(0);
}
