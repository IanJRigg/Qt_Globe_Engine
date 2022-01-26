#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    auto cameraAnglesHaveChanged = false;
    if(event->key() == Qt::Key_Left)
    {
        ui->GlobeRenderArea->decreaseCameraAzimuth();
        cameraAnglesHaveChanged = true;
    }

    if(event->key() == Qt::Key_Right)
    {
        ui->GlobeRenderArea->increaseCameraAzimuth();
        cameraAnglesHaveChanged = true;
    }

    if(event->key() == Qt::Key_Up)
    {
        ui->GlobeRenderArea->increaseCameraElevation();
        cameraAnglesHaveChanged = true;
    }

    if(event->key() == Qt::Key_Down)
    {
        ui->GlobeRenderArea->decreaseCameraElevation();
        cameraAnglesHaveChanged = true;
    }

    if(cameraAnglesHaveChanged)
    {
        ui->GlobeRenderArea->updateCameraPosition();
        ui->GlobeRenderArea->update();
    }
}

