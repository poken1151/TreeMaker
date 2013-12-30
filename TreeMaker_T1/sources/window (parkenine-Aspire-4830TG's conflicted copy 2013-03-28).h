#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>

#include "glwidget.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE
//! [0]
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

    void createBlockLayout();
    void createBasicLayout();
    void createCloudLayout();

public slots:
    void switchLayout(bool tim);
    void setTreeAgeSlider(int,int);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QSlider         *createSlider();
    QScrollArea     *scrollArea;
    QPushButton     *createButton();
    QStackedLayout  *allLayouts;
    QVBoxLayout     *noSelectLayout;
    QVBoxLayout     *blockSelectLayout;
    QVBoxLayout     *cloudSelectLayout;

    GLWidget *glWidget;
    QSlider *ageSlider;

    QPushButton *switchToMain;
    QPushButton *switchToCloud;
//    QSlider *ySlider;
//    QSlider *zSlider;
};
//! [0]

#endif
