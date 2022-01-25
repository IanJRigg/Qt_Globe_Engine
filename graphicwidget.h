#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QExposeEvent>

class GraphicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GraphicWidget(QWidget* parent = nullptr);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;
    void resizeEvent(QResizeEvent*) override;


private:
    void initOpenGL();

protected:
    std::unique_ptr<QOpenGLContext> m_context;
};

#endif // GRAPHICWIDGET_H
