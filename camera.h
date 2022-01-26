#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

class Camera
{
public:
    Camera();

    QMatrix4x4 viewMatrixAtPosition() const;
    QMatrix4x4 viewMatrixAtOrigin() const;
    QMatrix4x4 projectionMatrix(float aspectRatio) const;

    void setPosition(const QVector3D position);
    QVector3D position() const;

    float fieldOfView() const;
    void setFieldOfView(float fieldOfView);

    float distanceToNearPlane() const;
    void setDistanceToNearPlane(float distance);

    float distanceToFarPlane() const;
    void setDistanceToFarPlane(float distance);

private:
    QMatrix4x4 viewMatrixAt(const QVector3D& position) const;

private:
    QVector3D m_position;
    QQuaternion m_rotation;

    float m_fov;
    float m_distanceToNearPlane;
    float m_distanceToFarPlane;
};

#endif // CAMERA_H
