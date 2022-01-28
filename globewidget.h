#ifndef GLOBEWIDGET_H
#define GLOBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "shaderprogram.h"
#include "camera.h"

class GlobeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlobeWidget(QWidget* parent = nullptr);
    virtual ~GlobeWidget() override;

    void updateCameraRadius(float wheelInput);
    void updateCameraPositionAngles(float horizontalInput, float verticalInput);

    void enableWireframe();
    void disableWireframe();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int, int) override;

private:
    void initializeShaderProgram();
    void initializePlanetMesh();
    void initializeCamera();
    void initializeCubeMap();

    void updateAzimuth(float difference);
    void updateElevation(float difference);
    void updateRadius(float difference);

    void updateCameraPosition();

private:
    ShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_vertexArrayObject;
    QOpenGLBuffer m_vertexBufferObject;
    QOpenGLBuffer m_indexBufferObject;
    QOpenGLTexture m_texture;

    Camera m_camera;
    float m_cameraAzimuth;
    float m_cameraElevation;
    float m_cameraRadius;

    uint32_t m_numberOfSubdivisions;
    uint32_t m_numberOfIndices;
    bool m_renderingWireframe;
};

#endif // GLOBEWIDGET_H
