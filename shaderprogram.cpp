#include "shaderprogram.h"

#include <QOpenGLContext>

/**
 * \brief
 */
ShaderProgram::ShaderProgram() :
    m_programCreatedSuccessfully{false},
    m_layout{},
    m_program{nullptr}
{

}

/**
 * \brief
 */
void ShaderProgram::create(const QString &vertex_shader_path, const QString &fragment_shader_path)
{
    // Make sure that a context has been created prior to calling this function
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    // Create a new program object
    m_program.reset(new QOpenGLShaderProgram(QOpenGLContext::currentContext()));

    // Compile the vertex shader
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_shader_path))
    {
        qDebug() << "Unable to compile vertex shader: " << m_program->log();
        m_program.reset(nullptr);
        m_programCreatedSuccessfully = false;
        return;
    }

    // Compile the fragment shader
    if(!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_shader_path))
    {
        qDebug() << "Unable to compile fragment shader: " << m_program->log();
        m_program.reset(nullptr);
        m_programCreatedSuccessfully = false;
        return;
    }

    // Link the shaders
    if(!m_program->link())
    {
        qDebug() << "Unable to link shader program: " << m_program->log();
        m_program.reset(nullptr);
        m_programCreatedSuccessfully = false;
        return;
    }

    m_programCreatedSuccessfully = true;
}

/**
 * \brief
 */
void ShaderProgram::setAttribute(const int location,
                                 const GLenum type,
                                 const int offset,
                                 const int tupleSize,
                                 const int stride,
                                 const QString &locationName)
{
    // Make sure a program exists before calling this function
    Q_ASSERT(m_program != nullptr);

    // Set the attribute
    m_program->enableAttributeArray(location);
    m_program->setAttributeBuffer(location, type, offset, tupleSize, stride);

    // Update the layout structure
}

/**
 * \brief
 */
void ShaderProgram::setUniformMatrix(const QString& name, const QMatrix4x4& matrix)
{
    m_program->setUniformValue(name.toStdString().c_str(), matrix);
}

/**
 * \brief
 */
bool ShaderProgram::isCreated() const
{
    return m_programCreatedSuccessfully;
}

/**
 * \brief
 */
void ShaderProgram::bind() const
{
    // Make sure a program exists before calling this function
    Q_ASSERT(m_program != nullptr);

    m_program->bind();
}

/**
 * \brief
 */
QString ShaderProgram::memoryLayout() const
{
    // Make sure a program exists before calling this function
    Q_ASSERT(m_program != nullptr);

    return "";
}

