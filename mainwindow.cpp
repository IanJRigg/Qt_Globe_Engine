#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

namespace
{
    constexpr auto INPUT_HIGH = 1.0f;
    constexpr auto INPUT_HIGH_INVERTED = -1.0f;
    constexpr auto INPUT_LOW = 0.0f;

    constexpr auto ANGLE_DELTA_NO_INPUT = 0;
}

/**
 * \brief Standard constructor for a QMainWindow.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_globeRenderArea = ui->GlobeRenderArea;
}

/**
 * \brief Standard destructor for a QMainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * \brief Slot for the keyPressEvent. This allows the user to manipulate the azimuth/elevation of the camera.
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    float horizontalInput {INPUT_LOW};
    float verticalInput {INPUT_LOW};

    if(event->key() == Qt::Key_Left)
    {
        horizontalInput = INPUT_HIGH_INVERTED;
    }

    if(event->key() == Qt::Key_Right)
    {
        horizontalInput = INPUT_HIGH;
    }

    if(event->key() == Qt::Key_Down)
    {
        verticalInput = INPUT_HIGH_INVERTED;
    }

    if(event->key() == Qt::Key_Up)
    {
        verticalInput = INPUT_HIGH;
    }

    if(horizontalInput != INPUT_LOW || verticalInput != INPUT_LOW)
    {
        m_globeRenderArea->updateCameraPositionAngles(horizontalInput, verticalInput);
    }
}

/**
 * \brief Slot for the wheelEvent. This allows the user to manipulate the radius of the camera.
 *        The wheelInput sign is swapped here because the zoom in operation is subtractive, while
 *        the zoom out operation is additive.
 */
void MainWindow::wheelEvent(QWheelEvent* event)
{
    float wheelInput {INPUT_LOW};

    if(event->angleDelta().y() > ANGLE_DELTA_NO_INPUT)
    {
        wheelInput = INPUT_HIGH_INVERTED;
    }
    else if(event->angleDelta().y() < ANGLE_DELTA_NO_INPUT)
    {
        wheelInput = INPUT_HIGH;
    }

    m_globeRenderArea->updateCameraRadius(wheelInput);
}

/**
 * \brief Slot for the wireframe enable/disable action. Allows the user to turn on wireframe rendering.
 */
void MainWindow::on_Wireframe_On_Action_toggled(bool enabled)
{
    if(enabled)
    {
        m_globeRenderArea->enableWireframe();
    }
    else
    {
        m_globeRenderArea->disableWireframe();
    }
}

/**
 * \brief Slot for the quit action. Allows the user to exit the application.
 */
void MainWindow::on_Quit_Action_triggered()
{
    QApplication::quit();
}

