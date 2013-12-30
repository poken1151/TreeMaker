#include "window.h"

//! [0]
Window::Window()
{
    glWidget = new GLWidget;
    glWidget->setFixedHeight(600); //800
    glWidget->setFixedWidth(960);  //1280

    switchToMain = createButton();
        switchToMain->setText("To Main Layout");
//    xSlider = createSlider();
//    ySlider = createSlider();
//    zSlider = createSlider();

    connect(switchToMain, SIGNAL(clicked(bool)), this, SLOT(switchLayout(bool)));

//    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
//    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
//    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
//    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
//    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
//    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));
//! [0]

//! [1]
    //Widgets to hold the different layouts.
    QWidget *basicLayout = new QWidget();
    QWidget *blockLayout  = new QWidget();
    QWidget *cloudLayout = new QWidget();

    //ageSlider = new QSlider();

    //Initializing the class wide Layouts.
    QHBoxLayout *mainLayout = new QHBoxLayout;
    scrollArea = new QScrollArea;
    allLayouts = new QStackedLayout;
    noSelectLayout = new QVBoxLayout;
    blockSelectLayout = new QVBoxLayout;
    cloudSelectLayout = new QVBoxLayout;

    //In this section we are going to set up
    //all the specific widgets for each layout.
    ageSlider = createSlider();

    createBasicLayout();
    //noSelectLayout->addWidget(switchToCloud);

    createBlockLayout();

    createCloudLayout();

    //Setting all the layouts to their parent widgets.
    basicLayout->setLayout(noSelectLayout);
    blockLayout->setLayout(blockSelectLayout);
    cloudLayout->setLayout(cloudSelectLayout);

    //Setting all the layout widgets to the stacked layout.
    //Order: 0 - basix, 1 - cloud, 2 - block
    allLayouts->addWidget(basicLayout);
    allLayouts->addWidget(cloudLayout);
    allLayouts->addWidget(blockLayout);
    scrollArea->setLayout(allLayouts);

    //Defining the main Layout. Only two Items:
    //The GLWiget and the StackedLayout which is switchable.
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(ageSlider);
    mainLayout->addWidget(scrollArea);
    //mainLayout->addLayout(allLayouts);
    setLayout(mainLayout);

    allLayouts->setCurrentIndex(0);


    connect(glWidget, SIGNAL(ageLimitChanged(int,int)), this, SLOT(setTreeAgeSlider(int,int)));
    connect(ageSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setAgeLimit(int)));
    connect(glWidget, SIGNAL(sceneSelectionChanged(int)), allLayouts, SLOT(setCurrentIndex(int)));

    setWindowTitle(tr("TreeMaker Trial"));
}
//! [1]

//! [2]
QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 100);
    slider->setSingleStep(1);
    slider->setPageStep(10);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksLeft);
    return slider;
}


QPushButton *Window::createButton()
{
    QPushButton *button = new QPushButton();
//    button->setFixedHeight(5);
//    button->setFixedWidth(15);
    return button;
}
//! [2]

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
        QWidget::keyPressEvent(e);
}

void Window::createBasicLayout()
{
//    //Initializing the class wide Layouts.
//    QHBoxLayout *mainLayout = new QHBoxLayout;

    QLabel *colLable = new QLabel("Col Value");
    QLabel *cntLable = new QLabel("# of Nodes");
    QLabel *shpLable = new QLabel("Cloud Shape");
    QLabel *infLable = new QLabel("Influence Dist.");
    QLabel *kilLable = new QLabel("Kill Dist.");
    QLabel *xRaLable = new QLabel("X-Radius");
    QLabel *yRaLable = new QLabel("Y-Radius");
    QLabel *zRaLable = new QLabel("Z-Radius");
    QLabel *cSzLable = new QLabel("Cloud \"Size\"");
    QLabel *sedLable = new QLabel("Rndm Seed");

    QLabel *trnkLable = new QLabel("Initialize Trunk:");
    QLabel *xPosLable = new QLabel("X Pos.");
    QLabel *yPosLable = new QLabel("Y Pos.");
    QLabel *zPosLable = new QLabel("Z Pos.");
    QLabel *hiteLable = new QLabel("Trunk Height");    

    QLabel *blokLable = new QLabel("Create Block");
    QLabel *lentLable = new QLabel("Length");
    QLabel *widtLable = new QLabel("Width");
    QLabel *heitLable = new QLabel("Height");
    QLabel *axisLable = new QLabel("Main Axis");

    QLabel *stepLable = new QLabel("Step Size");


    QSpinBox *colSpinB = new QSpinBox;
    QSpinBox *cntSpinB = new QSpinBox;
    QSpinBox *shpSpinB = new QSpinBox;
    QDoubleSpinBox *infSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *kilSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *xRaSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *yRaSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *zRaSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *cSzSpinB = new QDoubleSpinBox;
    QSpinBox *sedSpinB = new QSpinBox;
    QPushButton *createCloudButton = new QPushButton("Add New Leaf Cloud");

    QDoubleSpinBox *xPosSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *yPosSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *zPosSpinB = new QDoubleSpinBox;
    QSpinBox *hiteSpinB = new QSpinBox;
    QPushButton *setTrunkButton = new QPushButton("Create Trunk");

    QDoubleSpinBox *lentSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *widtSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *heitSpinB = new QDoubleSpinBox;
    QSpinBox *axisSpinB = new QSpinBox;
    QPushButton *createBlockButton = new QPushButton("Create Block");

    QSpinBox *stepSpinB = new QSpinBox;
    QPushButton *growTreeButton = new QPushButton("Grow Tree");


    colSpinB->setRange(0,255);
    cntSpinB->setRange(1,7500);
    shpSpinB->setRange(0,3);
    infSpinB->setRange(15.0,100.0);
    kilSpinB->setRange(01.0,030.0);
    xRaSpinB->setRange(01.0,025.0);
    yRaSpinB->setRange(1.0,025.0);
    zRaSpinB->setRange(1.0,025.0);
    cSzSpinB->setRange(1.0,300.0);
    sedSpinB->setRange(1,99999999);

    xPosSpinB->setRange(-350.0,350.0);
    yPosSpinB->setRange(-30.0,30.0);
    zPosSpinB->setRange(-350.0,350.0);
    hiteSpinB->setRange(1,90);

    lentSpinB->setRange(0.0,400.0);
    widtSpinB->setRange(0.0,400.0);
    heitSpinB->setRange(0.0,400.0);
    axisSpinB->setRange(0,2);

    stepSpinB->setRange(1,9999);


    connect(colSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setColValue(int)));
    connect(cntSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setCntValue(int)));
    connect(shpSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setShpValue(int)));
    connect(infSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setInfValue(double)));
    connect(kilSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setKilValue(double)));
    connect(xRaSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXRaValue(double)));
    connect(yRaSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYRaValue(double)));
    connect(zRaSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZRaValue(double)));
    connect(cSzSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setCSzValue(double)));
    connect(sedSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setSedValue(int)));
    connect(createCloudButton, SIGNAL(clicked(bool)), glWidget, SLOT(createNewCloud(bool)));

    connect(xPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXPos(double)));
    connect(yPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYPos(double)));
    connect(zPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZPos(double)));
    connect(hiteSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setTrunkHeight(int)));
    connect(setTrunkButton, SIGNAL(clicked(bool)), glWidget, SLOT(setTrunk(bool)));

    connect(lentSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setBoxLen(double)));
    connect(widtSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setBoxWid(double)));
    connect(heitSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setBoxHgt(double)));
    connect(axisSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setBoxAxis(int)));
    connect(createBlockButton, SIGNAL(clicked(bool)), glWidget, SLOT(addBlock(bool)));

    connect(stepSpinB, SIGNAL(valueChanged(int)), glWidget, SLOT(setStepSize(int)));
    connect(growTreeButton, SIGNAL(clicked(bool)), glWidget, SLOT(growTreeOriginal(bool)));


    colSpinB->setValue(100);
    cntSpinB->setValue(3000);
    shpSpinB->setValue(2);
    infSpinB->setValue(45);
    kilSpinB->setValue(15);
    xRaSpinB->setValue(3);
    yRaSpinB->setValue(3);
    zRaSpinB->setValue(3);
    cSzSpinB->setValue(200);
    sedSpinB->setValue(2375836);

    hiteSpinB->setValue(45);

    lentSpinB->setValue(50);
    widtSpinB->setValue(10);
    heitSpinB->setValue(20);
    axisSpinB->setValue(2);

    stepSpinB->setValue(3000);


    QHBoxLayout *colBoxL = new QHBoxLayout;
    QHBoxLayout *cntBoxL = new QHBoxLayout;
    QHBoxLayout *shpBoxL = new QHBoxLayout;
    QHBoxLayout *infBoxL = new QHBoxLayout;
    QHBoxLayout *kilBoxL = new QHBoxLayout;
    QHBoxLayout *xRaBoxL = new QHBoxLayout;
    QHBoxLayout *yRaBoxL = new QHBoxLayout;
    QHBoxLayout *zRaBoxL = new QHBoxLayout;
    QHBoxLayout *cSzBoxL = new QHBoxLayout;
    QHBoxLayout *sedBoxL = new QHBoxLayout;

    QHBoxLayout *trnkBoxL = new QHBoxLayout;
    QHBoxLayout *xPosBoxL = new QHBoxLayout;
    QHBoxLayout *yPosBoxL = new QHBoxLayout;
    QHBoxLayout *zPosBoxL = new QHBoxLayout;
    QHBoxLayout *hiteBoxL = new QHBoxLayout;

    QHBoxLayout *blokBoxL = new QHBoxLayout;
    QHBoxLayout *lentBoxL = new QHBoxLayout;
    QHBoxLayout *widtBoxL = new QHBoxLayout;
    QHBoxLayout *heitBoxL = new QHBoxLayout;
    QHBoxLayout *axisBoxL = new QHBoxLayout;

    QHBoxLayout *stepBoxL = new QHBoxLayout;


    colBoxL->addWidget(colLable, Qt::AlignLeft);
    cntBoxL->addWidget(cntLable, Qt::AlignLeft);
    shpBoxL->addWidget(shpLable, Qt::AlignLeft);
    infBoxL->addWidget(infLable, Qt::AlignLeft);
    kilBoxL->addWidget(kilLable, Qt::AlignLeft);
    xRaBoxL->addWidget(xRaLable, Qt::AlignLeft);
    yRaBoxL->addWidget(yRaLable, Qt::AlignLeft);
    zRaBoxL->addWidget(zRaLable, Qt::AlignLeft);
    cSzBoxL->addWidget(cSzLable, Qt::AlignLeft);
    sedBoxL->addWidget(sedLable, Qt::AlignLeft);    

    trnkBoxL->addWidget(trnkLable);
    xPosBoxL->addWidget(xPosLable, Qt::AlignLeft);
    yPosBoxL->addWidget(yPosLable, Qt::AlignLeft);
    zPosBoxL->addWidget(zPosLable, Qt::AlignLeft);
    hiteBoxL->addWidget(hiteLable, Qt::AlignLeft);

    blokBoxL->addWidget(blokLable);
    lentBoxL->addWidget(lentLable, Qt::AlignLeft);
    widtBoxL->addWidget(widtLable, Qt::AlignLeft);
    heitBoxL->addWidget(heitLable, Qt::AlignLeft);
    axisBoxL->addWidget(axisLable, Qt::AlignLeft);

    stepBoxL->addWidget(stepLable, Qt::AlignLeft);


    colBoxL->addWidget(colSpinB, Qt::AlignRight);
    cntBoxL->addWidget(cntSpinB, Qt::AlignRight);
    shpBoxL->addWidget(shpSpinB, Qt::AlignRight);
    infBoxL->addWidget(infSpinB, Qt::AlignRight);
    kilBoxL->addWidget(kilSpinB, Qt::AlignRight);
    xRaBoxL->addWidget(xRaSpinB, Qt::AlignRight);
    yRaBoxL->addWidget(yRaSpinB, Qt::AlignRight);
    zRaBoxL->addWidget(zRaSpinB, Qt::AlignRight);
    cSzBoxL->addWidget(cSzSpinB, Qt::AlignRight);
    sedBoxL->addWidget(sedSpinB, Qt::AlignRight);

    xPosBoxL->addWidget(xPosSpinB, Qt::AlignRight);
    yPosBoxL->addWidget(yPosSpinB, Qt::AlignRight);
    zPosBoxL->addWidget(zPosSpinB, Qt::AlignRight);
    hiteBoxL->addWidget(hiteSpinB, Qt::AlignRight);

    lentBoxL->addWidget(lentSpinB, Qt::AlignRight);
    widtBoxL->addWidget(widtSpinB, Qt::AlignRight);
    heitBoxL->addWidget(heitSpinB, Qt::AlignRight);
    axisBoxL->addWidget(axisSpinB, Qt::AlignRight);

    stepBoxL->addWidget(stepSpinB, Qt::AlignRight);


    noSelectLayout->addLayout(colBoxL);
    noSelectLayout->addLayout(cntBoxL);
    noSelectLayout->addLayout(shpBoxL);
    noSelectLayout->addLayout(infBoxL);
    noSelectLayout->addLayout(kilBoxL);
    noSelectLayout->addLayout(xRaBoxL);
    noSelectLayout->addLayout(yRaBoxL);
    noSelectLayout->addLayout(zRaBoxL);
    noSelectLayout->addLayout(cSzBoxL);
    noSelectLayout->addLayout(sedBoxL);
    noSelectLayout->addWidget(createCloudButton);
    noSelectLayout->addLayout(trnkBoxL);
    noSelectLayout->addLayout(xPosBoxL);
    noSelectLayout->addLayout(yPosBoxL);
    noSelectLayout->addLayout(zPosBoxL);
    noSelectLayout->addLayout(hiteBoxL);
    noSelectLayout->addWidget(setTrunkButton);
    noSelectLayout->addLayout(blokBoxL);
    noSelectLayout->addLayout(lentBoxL);
    noSelectLayout->addLayout(widtBoxL);
    noSelectLayout->addLayout(heitBoxL);
    noSelectLayout->addLayout(axisBoxL);
    noSelectLayout->addWidget(createBlockButton);
    noSelectLayout->addLayout(stepBoxL);
    noSelectLayout->addWidget(growTreeButton);
    noSelectLayout->addStretch(0);
}

void Window::createCloudLayout()
{
    //    //Initializing the class wide Layouts.
    //    QHBoxLayout *mainLayout = new QHBoxLayout;

        QLabel *xPosLable = new QLabel("X Pos.");
        QLabel *yPosLable = new QLabel("Y Pos.");
        QLabel *zPosLable = new QLabel("Z Pos.");

        QDoubleSpinBox *xPosSpinB = new QDoubleSpinBox;
        QDoubleSpinBox *yPospinB = new QDoubleSpinBox;
        QDoubleSpinBox *zPosSpinB = new QDoubleSpinBox;
        QPushButton *deleteCloudButton = new QPushButton("Delete Selected Cloud");

        xPosSpinB->setRange(-350.0,350.0);
        yPospinB->setRange(-125.0,125.0);
        zPosSpinB->setRange(-350.0,350.0);

        connect(xPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXPos(double)));
        connect(yPospinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYPos(double)));
        connect(zPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZPos(double)));
        connect(glWidget, SIGNAL(xPosChanged(double)), xPosSpinB, SLOT(setValue(double)));
        connect(glWidget, SIGNAL(yPosChanged(double)), yPospinB, SLOT(setValue(double)));
        connect(glWidget, SIGNAL(zPosChanged(double)), zPosSpinB, SLOT(setValue(double)));
        connect(deleteCloudButton, SIGNAL(clicked(bool)), glWidget, SLOT(deleteObject(bool)));

        QHBoxLayout *xPosBoxL = new QHBoxLayout;
        QHBoxLayout *yPosBoxL = new QHBoxLayout;
        QHBoxLayout *zPosBoxL = new QHBoxLayout;

        xPosBoxL->addWidget(xPosLable, Qt::AlignLeft);
        yPosBoxL->addWidget(yPosLable, Qt::AlignLeft);
        zPosBoxL->addWidget(zPosLable, Qt::AlignLeft);

        xPosBoxL->addWidget(xPosSpinB, Qt::AlignRight);
        yPosBoxL->addWidget(yPospinB, Qt::AlignRight);
        zPosBoxL->addWidget(zPosSpinB, Qt::AlignRight);

        cloudSelectLayout->addLayout(xPosBoxL);
        cloudSelectLayout->addLayout(yPosBoxL);
        cloudSelectLayout->addLayout(zPosBoxL);
        cloudSelectLayout->addWidget(deleteCloudButton);
        cloudSelectLayout->addStretch(0);

}

void Window::createBlockLayout()
{
    QLabel *xPosLable = new QLabel("X Pos.");
    QLabel *yPosLable = new QLabel("Y Pos.");
    QLabel *zPosLable = new QLabel("Z Pos.");

    QDoubleSpinBox *xPosSpinB = new QDoubleSpinBox;
    QDoubleSpinBox *yPospinB = new QDoubleSpinBox;
    QDoubleSpinBox *zPosSpinB = new QDoubleSpinBox;
    QPushButton *deleteBoxButton = new QPushButton("Delete Selected Block");

    xPosSpinB->setRange(-350.0,350.0);
    yPospinB->setRange(-125.0,125.0);
    zPosSpinB->setRange(-350.0,350.0);

    connect(xPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setXPos(double)));
    connect(yPospinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setYPos(double)));
    connect(zPosSpinB, SIGNAL(valueChanged(double)), glWidget, SLOT(setZPos(double)));
    connect(glWidget, SIGNAL(xPosChanged(double)), xPosSpinB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(yPosChanged(double)), yPospinB, SLOT(setValue(double)));
    connect(glWidget, SIGNAL(zPosChanged(double)), zPosSpinB, SLOT(setValue(double)));
    connect(deleteBoxButton, SIGNAL(clicked(bool)), glWidget, SLOT(deleteObject(bool)));

    QHBoxLayout *xPosBoxL = new QHBoxLayout;
    QHBoxLayout *yPosBoxL = new QHBoxLayout;
    QHBoxLayout *zPosBoxL = new QHBoxLayout;

    xPosBoxL->addWidget(xPosLable, Qt::AlignLeft);
    yPosBoxL->addWidget(yPosLable, Qt::AlignLeft);
    zPosBoxL->addWidget(zPosLable, Qt::AlignLeft);

    xPosBoxL->addWidget(xPosSpinB, Qt::AlignRight);
    yPosBoxL->addWidget(yPospinB, Qt::AlignRight);
    zPosBoxL->addWidget(zPosSpinB, Qt::AlignRight);

    blockSelectLayout->addLayout(xPosBoxL);
    blockSelectLayout->addLayout(yPosBoxL);
    blockSelectLayout->addLayout(zPosBoxL);
    blockSelectLayout->addWidget(deleteBoxButton);
    blockSelectLayout->addStretch(0);
}
