#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GraphicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GraphicWindow();
};

#endif // GRAPHICWINDOW_H
