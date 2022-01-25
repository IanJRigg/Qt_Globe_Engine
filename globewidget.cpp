#include "globewidget.h"
#include "planetgenerator.h"

/**
 * \brief
 */
GlobeWidget::GlobeWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    m_shaderProgram()
{

}

/**
 * \brief
 */
void GlobeWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Create the shader program
    m_shaderProgram.create(":/cube-map.vert", ":/cube-map.frag");

    // Create the VBO
    m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBufferObject.create();
    m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(m_vertexBufferObject.isCreated())
    {
        qDebug() << "Could not create VBO!";
    }

    // Create the Vertex Index Object
    m_indexBufferObject = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_indexBufferObject.create();
    m_indexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    // Create the VAO
    m_vertexArrayObject.create();
    if(!m_vertexArrayObject.isCreated())
    {
        qDebug() << "Could not create VAO!";
    }

    const auto [vertices, indices] = generateSubdividedCube(1);

    m_vertexArrayObject.bind();
    m_vertexBufferObject.bind();
    m_indexBufferObject.bind();

    m_vertexBufferObject.allocate(vertices.data(), vertices.size() * sizeof(float));
    m_indexBufferObject.allocate(indices.data(), indices.size() * sizeof(indices) * sizeof(uint32_t));

    m_shaderProgram.setAttribute(0, GL_FLOAT, 0, 3, 6 * sizeof(float), "Vertices");

    m_indexBufferObject.release();
    m_vertexBufferObject.release();
    m_vertexArrayObject.release();
}

/**
 * \brief
 */
void GlobeWidget::paintGL()
{

}

/**
 * \brief Triggered when the window resizes. The only purpose of this function wil be to
 *        adjust the MVP matrices as needed for this change.
 * \param w
 * \param h
 */
void GlobeWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void GlobeWidget::setupOpenGL()
{

}
