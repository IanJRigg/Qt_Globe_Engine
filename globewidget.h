#ifndef GLOBEWIDGET_H
#define GLOBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "shaderprogram.h"

class GlobeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlobeWidget(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    ShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_vertexArrayObject;
    QOpenGLBuffer m_vertexBufferObject;
    QOpenGLBuffer m_indexBufferObject;

    uint32_t m_numberOfIndices;
    bool m_renderingWireframe;
};

#endif // GLOBEWIDGET_H
