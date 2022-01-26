#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <memory>
#include <QString>
#include <QOpenGLShaderProgram>

class ShaderProgram
{
public:
    ShaderProgram();

    void create(const QString& vertex_shader_path, const QString& fragment_shader_path);
    void setAttribute(int location,
                      GLenum type,
                      int offset,
                      int tupleSize,
                      int stride = 0,
                      const QString& locationName = "");
    void setUniformMatrix(const QString& name, const QMatrix4x4& mvp);

    bool isCreated() const;
    void bind() const;
    QString memoryLayout() const;

private:
    bool m_programCreatedSuccessfully;
    QList<QPair<int, QString>> m_layout;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
};

#endif // SHADERPROGRAM_H
