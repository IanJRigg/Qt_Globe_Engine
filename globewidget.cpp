#include "globewidget.h"
#include "planetgenerator.h"

#include <QtMath>

namespace
{
    const QString VERTEX_SHADER_PATH = ":/shaders/cube-map.vert";
    const QString FRAGMENT_SHADER_PATH = ":/shaders/cube-map.frag";

    const QString CUBEMAP_POSITIVE_X_PATH = ":/textures/asia.png";
    const QString CUBEMAP_NEGATIVE_X_PATH = ":/textures/americas.png";
    const QString CUBEMAP_POSITIVE_Y_PATH = ":/textures/arctic.png";
    const QString CUBEMAP_NEGATIVE_Y_PATH = ":/textures/antarctica.png";
    const QString CUBEMAP_POSITIVE_Z_PATH = ":/textures/africa.png";
    const QString CUBEMAP_NEGATIVE_Z_PATH = ":/textures/pacific.png";
}

/**
 * \brief
 */
GlobeWidget::GlobeWidget(QWidget* parent) :
    QOpenGLWidget{parent},
    m_shaderProgram{},
    m_vertexArrayObject{},
    m_vertexBufferObject{QOpenGLBuffer::VertexBuffer}, // constructor is pass through, no OpenGL initialization required
    m_indexBufferObject{QOpenGLBuffer::IndexBuffer}, // constructor is pass through, no OpenGL initialization required
    m_texture{QOpenGLTexture::TargetCubeMap}, // Constructor is pass throguh, no OpenGL initialization required
    m_camera{},
    m_cameraAzimuth{0.0f},
    m_cameraElevation{0.0},
    m_cameraRadius{7.5f},
    m_numberOfSubdivisions{15},
    m_numberOfIndices{0},
    m_renderingWireframe{false}
{

}

/**
 * \brief
 */
GlobeWidget::~GlobeWidget()
{
    // Context must be made current because there's no guarantee of which context is current at destruction time
    makeCurrent();

    m_vertexArrayObject.destroy();
    m_vertexBufferObject.destroy();
    m_indexBufferObject.destroy();
    m_texture.destroy();
}

void GlobeWidget::updateCameraPosition()
{
    QVector3D newCameraPosition;
    newCameraPosition.setY(m_cameraRadius * qSin(qDegreesToRadians(m_cameraElevation)));
    auto hypotenuse = m_cameraRadius * qCos(qDegreesToRadians(m_cameraElevation));

    newCameraPosition.setX(hypotenuse * qSin(qDegreesToRadians(m_cameraAzimuth)));
    newCameraPosition.setZ(hypotenuse * qCos(qDegreesToRadians(m_cameraAzimuth)));

    m_camera.setPosition(newCameraPosition);
}

/**
 * \brief
 */
void GlobeWidget::increaseCameraAzimuth()
{
    updateAzimuth(10.0f);
}

/**
 * \brief
 */
void GlobeWidget::decreaseCameraAzimuth()
{
    updateAzimuth(-10.0f);
}

/**
 * \brief
 */
void GlobeWidget::zoomIn()
{
    if(m_cameraRadius > 2.4f)
    {
        m_cameraRadius -= 0.2f;
    }
}

/**
 * \brief
 */
void GlobeWidget::zoomOut()
{
    if(m_cameraRadius < 7.5f)
    {
        m_cameraRadius += 0.2f;
    }
}

/**
 * \brief
 */
void GlobeWidget::increaseCameraElevation()
{
    updateElevation(5.0f);
}

/**
 * \brief
 */
void GlobeWidget::decreaseCameraElevation()
{
    updateElevation(-5.0f);
}

/**
 * \brief
 */
void GlobeWidget::enableWireframe()
{
    m_renderingWireframe = true;
}

/**
 * \brief
 */
void GlobeWidget::disableWireframe()
{
    m_renderingWireframe = false;
}

/**
 * \brief
 */
void GlobeWidget::initializeGL()
{
    // Qt function that MUST be done prior to any OpenGL function calls
    initializeOpenGLFunctions();

    // Enable backface culling. This prevents the far face of the sphere from being simulatneously visible with the front face
    glEnable(GL_CULL_FACE);

    // Custom calls to setup the scene
    initializeShaderProgram();
    initializePlanetMesh();
    initializeCubeMap();
    initializeCamera();
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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    m_shaderProgram.bind();

    // Bind the vertex array object. This binds the vertex buffer object and sets the attribute buffer in the OpenGL context
    m_vertexArrayObject.bind();

    m_texture.bind();

    // Generate the MVP Matrix and pass it to the shaders
    QMatrix4x4 model;
    auto view = m_camera.viewMatrixAtPosition();

    auto aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    auto projection = m_camera.projectionMatrix(aspectRatio);

    m_shaderProgram.setUniformMatrix("mvp", projection * view * model);

    // now draw the two triangles via index drawing
    if(m_renderingWireframe)
    {
        glDrawElements(GL_LINES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }

    m_texture.release();
    m_vertexArrayObject.release();
    // m_shaderProgram.release();
}

/**
 * \brief Triggered when the window resizes. The only purpose of this function wil be to
 *        adjust the MVP matrices as needed for this change.
 */
void GlobeWidget::resizeGL(int, int)
{
    paintGL();
}

/**
 * \brief
 */
void GlobeWidget::initializeShaderProgram()
{
    m_shaderProgram.create(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

/**
 * \brief
 */
void GlobeWidget::initializePlanetMesh()
{
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

    // NOTE: the following will assert if this function is called before the shaderProgram is initialized
    // Assign position 0 to be the vertices of the globe
    auto stride = (3 * sizeof(float));
    m_shaderProgram.setAttribute(0, GL_FLOAT, 0, 3, stride, "Vertices");

    // Record the number of indices used in the operation above. Needed for glDrawElements()
    m_numberOfIndices = indices.size();

    m_vertexArrayObject.release();
    m_vertexBufferObject.release();
    m_indexBufferObject.release();
}

/**
  * \brief
  */
void GlobeWidget::initializeCamera()
{
    m_camera.setFieldOfView(20.0f);
    m_camera.setDistanceToNearPlane(0.1f);
    m_camera.setDistanceToFarPlane(10.0f);
}

/**
 * \brief
 */
void GlobeWidget::initializeCubeMap()
{
    auto positiveX = QImage(CUBEMAP_POSITIVE_X_PATH).convertToFormat(QImage::Format_RGBA8888);
    auto negativeX = QImage(CUBEMAP_NEGATIVE_X_PATH).convertToFormat(QImage::Format_RGBA8888);
    auto positiveY = QImage(CUBEMAP_POSITIVE_Y_PATH).convertToFormat(QImage::Format_RGBA8888);
    auto negativeY = QImage(CUBEMAP_NEGATIVE_Y_PATH).convertToFormat(QImage::Format_RGBA8888);
    auto positiveZ = QImage(CUBEMAP_POSITIVE_Z_PATH).convertToFormat(QImage::Format_RGBA8888);
    auto negativeZ = QImage(CUBEMAP_NEGATIVE_Z_PATH).convertToFormat(QImage::Format_RGBA8888);

    m_texture.create();
    m_texture.setSize(positiveX.width(), positiveX.height(), positiveX.depth());
    m_texture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture.allocateStorage();

    m_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, positiveX.constBits());
    m_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negativeX.constBits());
    m_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, positiveY.constBits());
    m_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negativeY.constBits());
    m_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, positiveZ.constBits());
    m_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negativeZ.constBits());

    m_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    m_shaderProgram.setUniformValue("CubeMap", 0);
}

/**
 * \brief
 */
void GlobeWidget::updateAzimuth(const float difference)
{
    // Input sanitization
    Q_ASSERT(difference <= 360.0f && difference >= -360.0f);

    // Wrap the azimuth once it's gone over
    m_cameraAzimuth += difference;
    if(m_cameraAzimuth > 360.0f || m_cameraAzimuth < -360.0f)
    {
        m_cameraAzimuth = 0.0f;
    }
}

/**
 * \brief
 */
void GlobeWidget::updateElevation(float difference)
{
    // Input sanitization
    Q_ASSERT(difference <= 80.0f && difference >= -80.0f);

    // Bail out if you're too close to the poles
    m_cameraElevation += difference;
    if(m_cameraElevation >= 80.0f)
    {
        m_cameraElevation = 80.0f;
    }
    else if(m_cameraElevation < -80.0f)
    {
        m_cameraElevation = -80.0f;
    }
}
