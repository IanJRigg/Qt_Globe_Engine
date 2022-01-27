#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

/**
 * \brief
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * \brief
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * \brief
 */
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

/**
 * \brief
 */
void MainWindow::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() > 0)
    {
        ui->GlobeRenderArea->zoomIn();
    }
    else if(event->angleDelta().y() < 0)
    {
        ui->GlobeRenderArea->zoomOut();
    }

    ui->GlobeRenderArea->updateCameraPosition();
    ui->GlobeRenderArea->update();
}

/**
 * \brief
 */
void MainWindow::on_Wireframe_On_Action_toggled(bool enabled)
{
    if(enabled)
    {
        ui->GlobeRenderArea->enableWireframe();
    }
    else
    {
        ui->GlobeRenderArea->disableWireframe();
    }

    ui->GlobeRenderArea->update();
}

/**
 * \brief
 */
void MainWindow::on_Quit_Action_triggered()
{
    QApplication::quit();
}

