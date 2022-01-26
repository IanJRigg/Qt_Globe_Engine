#include "globewidget.h"
#include "planetgenerator.h"

/**
 * \brief
 */
GlobeWidget::GlobeWidget(QWidget* parent) :
    QOpenGLWidget{parent},
    m_shaderProgram{},
    m_vertexArrayObject{},
    m_vertexBufferObject{QOpenGLBuffer::VertexBuffer}, // constructor is pass through, no OpenGL initialization required
    m_indexBufferObject{QOpenGLBuffer::IndexBuffer}, // constructor is pass through, no OpenGL initialization required
    m_camera{},
    m_numberOfSubdivisions{10},
    m_numberOfIndices{0},
    m_renderingWireframe{true}
{

}

GlobeWidget::~GlobeWidget()
{
    m_vertexArrayObject.destroy();
    m_vertexBufferObject.destroy();
    m_indexBufferObject.destroy();
}

/**
 * \brief
 */
void GlobeWidget::initializeGL()
{
    // Make the initialization call here. This MUST be done prior to any OpenGL function calls
    initializeOpenGLFunctions();

    // Create the shader program
    m_shaderProgram.create(":/shaders/cube-map.vert", ":/shaders/cube-map.frag");
    // m_shaderProgram.create(":/shaders/pass_through.vert", ":/shaders/uniform_color.frag");

    // Create the VBO
    m_vertexBufferObject.create();
    m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(!m_vertexBufferObject.isCreated())
    {
        qDebug() << "Could not create VBO!";
    }

    // Create the Vertex Index Object
    m_indexBufferObject.create();
    m_indexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(!m_indexBufferObject.isCreated())
    {
        qDebug() << "Could not create index buffer object";
    }

    // Create the VAO
    m_vertexArrayObject.create();
    if(!m_vertexArrayObject.isCreated())
    {
        qDebug() << "Could not create VAO!";
    }

    // Generate the model for the sphere
    const auto [vertices, indices] = generateSubdividedCube(m_numberOfSubdivisions);

    m_vertexArrayObject.bind();
    m_vertexBufferObject.bind();
    m_indexBufferObject.bind();

    // Allocate the memory needed for the vertex and index buffers respectively
    m_vertexBufferObject.allocate(vertices.data(), vertices.size() * sizeof(float));
    m_indexBufferObject.allocate(indices.data(), indices.size() * sizeof(uint32_t));

    // Assign position 0 to be the vertices of the globe
    auto stride = (3 * sizeof(float));
    m_shaderProgram.setAttribute(0, GL_FLOAT, 0, 3, stride, "Vertices");

    // Record the number of indices used in the operation above. Needed for glDrawElements()
    m_numberOfIndices = indices.size();

    m_vertexArrayObject.release();
    m_vertexBufferObject.release();
    m_indexBufferObject.release();

    // Setup the camera
    m_camera.setFieldOfView(45.0f);
    m_camera.setDistanceToNearPlane(0.1f);
    m_camera.setDistanceToFarPlane(10.0f);
}

/**
 * \brief
 */
void GlobeWidget::paintGL()
{
    // Needed for every frame that's rendered to screen
    const auto retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Set the background color = clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program
    m_shaderProgram.bind();

    // Bind the vertex array object. This binds the vertex buffer object and sets the attribute buffer in the OpenGL context
    m_vertexArrayObject.bind();

    // Update the MVP Matrix
    QMatrix4x4 modelMatrix; // Just the identity here
    auto viewMatrix = m_camera.viewMatrixAtPosition();
    auto projectionMatrix = m_camera.projectionMatrix(16.0f / 9.0f); // TODO: Calculate this instead of using a constant

    auto mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
    m_shaderProgram.setMVPMatrix(mvpMatrix);

    // now draw the two triangles via index drawing
    if(m_renderingWireframe)
    {
        glDrawElements(GL_LINES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }

    m_vertexArrayObject.release();
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
