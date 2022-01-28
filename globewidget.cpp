#include "globewidget.h"
#include "planetgenerator.h"

#include <QtMath>

// Local constants
namespace
{
    const auto VERTEX_SHADER_PATH = ":/shaders/cube-map.vert";
    const auto FRAGMENT_SHADER_PATH = ":/shaders/cube-map.frag";

    const auto CUBEMAP_POSITIVE_X_PATH = ":/textures/asia.png";
    const auto CUBEMAP_NEGATIVE_X_PATH = ":/textures/americas.png";
    const auto CUBEMAP_POSITIVE_Y_PATH = ":/textures/arctic.png";
    const auto CUBEMAP_NEGATIVE_Y_PATH = ":/textures/antarctica.png";
    const auto CUBEMAP_POSITIVE_Z_PATH = ":/textures/africa.png";
    const auto CUBEMAP_NEGATIVE_Z_PATH = ":/textures/pacific.png";

    const auto MVP_MATRIX_NAME_IN_SHADERS = "mvp";
    const auto CUBEMAP_NAME_IN_SHADERS = "CubeMap";

    constexpr auto NUMBER_OF_SUBDIVISIONS = 15;

    constexpr auto RADIUS_LOWER_LIMIT = 2.4f;
    constexpr auto RADIUS_UPPER_LIMIT = 7.5f;
    constexpr auto RADIUS_INCREMENT = 0.2f;

    constexpr auto AZIMUTH_ORIGIN = 0.0f;
    constexpr auto AZIMUTH_LOWER_LIMIT = -360.0f;
    constexpr auto AZIMUTH_UPPER_LIMIT = 360.0f;
    constexpr auto AZIMUTH_INCREMENT = 10.0f;

    constexpr auto ELEVATION_ORIGIN = 0.0f;
    constexpr auto ELEVATION_LOWER_LIMIT = -80.0f;
    constexpr auto ELEVATION_UPPER_LIMIT = 80.0f;
    constexpr auto ELEVATION_INCREMENT = 10.0f;

    constexpr auto DEFAULT_FIELD_OF_VIEW = 20.0f;
    constexpr auto DEFAULT_NEAR_PLANE_DISTANCE = 0.1f;
    constexpr auto DEFAULT_FAR_PLANE_DISTANCE = 10.0f;
}

/**
 * \brief Constructor for the globe widget. Purely used for assignment, no functions should be called here.
 */
GlobeWidget::GlobeWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    m_shaderProgram(),
    m_vertexArrayObject(),
    m_vertexBufferObject(QOpenGLBuffer::VertexBuffer), // constructor is pass through, no OpenGL initialization required
    m_indexBufferObject(QOpenGLBuffer::IndexBuffer), // constructor is pass through, no OpenGL initialization required
    m_texture(QOpenGLTexture::TargetCubeMap), // Constructor is pass throguh, no OpenGL initialization required
    m_camera(0.0f, 0.0f, RADIUS_UPPER_LIMIT),
    m_cameraAzimuth{AZIMUTH_ORIGIN},
    m_cameraElevation{ELEVATION_ORIGIN},
    m_cameraRadius{RADIUS_UPPER_LIMIT},
    m_numberOfSubdivisions{NUMBER_OF_SUBDIVISIONS},
    m_numberOfIndices{0},
    m_renderingWireframe{false}
{

}

/**
 * \brief Destructor for the globe widget. The context must be made current because there's
 *        no guarantee of which context is current at destruction time
 */
GlobeWidget::~GlobeWidget()
{
    makeCurrent();

    m_vertexArrayObject.destroy();
    m_vertexBufferObject.destroy();
    m_indexBufferObject.destroy();
    m_texture.destroy();
}

/**
 * \brief Mutator for the camera radius. Radius is updated, the camera position is
 *        recalculated, and then the scene is re-drawn
 */
void GlobeWidget::updateCameraRadius(const float wheelInput)
{
    updateRadius(RADIUS_INCREMENT * wheelInput);
    updateCameraPosition();

    this->update();
}

/**
 * \brief Mutator for the camera position angles. Azimuth and Elevation are updated, the
 *        camera position is recalculatued, and then the scene is re-drawn.
 */
void GlobeWidget::updateCameraPositionAngles(const float horizontalInput, const float verticalInput)
{
    updateAzimuth(AZIMUTH_INCREMENT * horizontalInput);
    updateElevation(ELEVATION_INCREMENT * verticalInput);

    updateCameraPosition();

    this->update();
}

/**
 * \brief Basic mutator for the m_renderingWireframe. Calls the parent object's update function
 *        after making the change
 */
void GlobeWidget::enableWireframe()
{
    m_renderingWireframe = true;
    this->update();
}

/**
 * \brief Basic mutator for the m_renderingWireframe. Calls the parent object's update function
 *        after making the change
 */
void GlobeWidget::disableWireframe()
{
    m_renderingWireframe = false;
    this->update();
}

/**
 * \brief Standardized function when using OpenGL with Qt. All initialization that requires
 *        OpenGL function calls should be done here.
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
 * \brief Standardized function when using OpenGL with Qt. Any time the scene changes and needs
 *        to be re-drawn, this function should be called.
 */
void GlobeWidget::paintGL()
{
    // Needed for every frame that's rendered to screen
    const auto retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Set the background color to a dark black
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the relevant OpenGL objects
    m_shaderProgram.bind();
    m_vertexArrayObject.bind();
    m_texture.bind();

    // Generate the MVP Matrix and pass it to the shaders
    QMatrix4x4 model;
    auto view = m_camera.viewMatrixAtPosition();

    auto aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    auto projection = m_camera.projectionMatrix(aspectRatio);

    m_shaderProgram.setUniformMatrix(MVP_MATRIX_NAME_IN_SHADERS, projection * view * model);

    // now draw the two triangles via index drawing
    if(m_renderingWireframe)
    {
        glDrawElements(GL_LINES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, m_numberOfIndices, GL_UNSIGNED_INT, nullptr);
    }

    // Release the relevant OpenGL objects
    m_texture.release();
    m_vertexArrayObject.release();
    m_shaderProgram.release();
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
 * \brief Utility function to handle creation of m_shaderProgram and anything else related to it.
 */
void GlobeWidget::initializeShaderProgram()
{
    m_shaderProgram.create(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

/**
 * \brief Utility function to handle creation of m_vertexBufferObject, m_indexBufferObject, and
 *        m_vertexArrayObject. After creation, the buffers are provided to the scripts.
 */
void GlobeWidget::initializePlanetMesh()
{
    // Ensure that m_shaderProgram has been initialized prior to continuing
    Q_ASSERT(m_shaderProgram.isCreated());

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
    m_shaderProgram.setAttribute(0, GL_FLOAT, 0, 3, stride);

    // Record the number of indices used in the operation above. Needed for glDrawElements()
    m_numberOfIndices = indices.size();

    m_vertexArrayObject.release();
    m_vertexBufferObject.release();
    m_indexBufferObject.release();
}

/**
  * \brief Utility function to handle creation of m_camera.
  */
void GlobeWidget::initializeCamera()
{
    m_camera.setFieldOfView(DEFAULT_FIELD_OF_VIEW);
    m_camera.setDistanceToNearPlane(DEFAULT_NEAR_PLANE_DISTANCE);
    m_camera.setDistanceToFarPlane(DEFAULT_FAR_PLANE_DISTANCE);
}

/**
 * \brief Utility function to handle creation of m_texture as a cubemap.
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

    m_shaderProgram.setUniformValue(CUBEMAP_NAME_IN_SHADERS, 0);
}

/**
 * \brief Utility function to sanitize changes to m_cameraAzimuth.
 */
void GlobeWidget::updateAzimuth(const float difference)
{
    // Input sanitization
    Q_ASSERT(difference <= AZIMUTH_UPPER_LIMIT && difference >= AZIMUTH_LOWER_LIMIT);

    // Wrap the azimuth once it's gone over
    m_cameraAzimuth += difference;
    if(m_cameraAzimuth > AZIMUTH_UPPER_LIMIT || m_cameraAzimuth < AZIMUTH_LOWER_LIMIT)
    {
        m_cameraAzimuth = AZIMUTH_ORIGIN;
    }
}

/**
 * \brief Utility function to sanitize changes to m_cameraElevation
 */
void GlobeWidget::updateElevation(const float difference)
{
    // Input sanitization
    Q_ASSERT(difference <= ELEVATION_UPPER_LIMIT &&
             difference >= ELEVATION_LOWER_LIMIT);

    // Bail out if you're too close to the poles
    m_cameraElevation += difference;
    if(m_cameraElevation > ELEVATION_UPPER_LIMIT)
    {
        m_cameraElevation = ELEVATION_UPPER_LIMIT;
    }
    else if(m_cameraElevation < ELEVATION_LOWER_LIMIT)
    {
        m_cameraElevation = ELEVATION_LOWER_LIMIT;
    }
}

/**
 * \brief Utility function to santize changes to m_cameraRadius
 */
void GlobeWidget::updateRadius(const float difference)
{
    Q_ASSERT(difference <= 1.0f && difference >= -1.0f);

    m_cameraRadius += difference;
    if(m_cameraRadius > RADIUS_UPPER_LIMIT)
    {
        m_cameraRadius = RADIUS_UPPER_LIMIT;
    }
    else if(m_cameraRadius < RADIUS_LOWER_LIMIT)
    {
        m_cameraRadius = RADIUS_LOWER_LIMIT;
    }
}

/**
 * \brief Utility function that performs a conversion from sphereical coordinates
 *        to cartesian, right hand XYZ
 */
void GlobeWidget::updateCameraPosition()
{
    QVector3D newCameraPosition;
    newCameraPosition.setY(m_cameraRadius * qSin(qDegreesToRadians(m_cameraElevation)));
    auto hypotenuse = m_cameraRadius * qCos(qDegreesToRadians(m_cameraElevation));

    newCameraPosition.setX(hypotenuse * qSin(qDegreesToRadians(m_cameraAzimuth)));
    newCameraPosition.setZ(hypotenuse * qCos(qDegreesToRadians(m_cameraAzimuth)));

    m_camera.setPosition(newCameraPosition);
}
