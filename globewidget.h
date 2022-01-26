#ifndef GLOBEWIDGET_H
#define GLOBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QKeyEvent>

#include "shaderprogram.h"
#include "camera.h"

class GlobeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlobeWidget(QWidget* parent = nullptr);
    virtual ~GlobeWidget() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

public:
    void rotateCamera();

private:
    ShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_vertexArrayObject;
    QOpenGLBuffer m_vertexBufferObject;
    QOpenGLBuffer m_indexBufferObject;

    Camera m_camera;
    float m_cameraAzimuth;
    float m_cameraRadius;

    uint32_t m_numberOfSubdivisions;
    uint32_t m_numberOfIndices;
    bool m_renderingWireframe;
};

#endif // GLOBEWIDGET_H
