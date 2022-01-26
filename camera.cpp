#include "camera.h"

namespace
{
    QVector3D ORIGIN{ 0.0f, 0.0f, 0.0f };
    QVector3D Y_AXIS{ 0.0f, 1.0f, 0.0f };
}

/**
 * \brief
 */
Camera::Camera() :
    m_position{0.0f, 0.0f, 2.0f},
    m_rotation{},
    m_fov{0.0f},
    m_distanceToNearPlane{0.0f},
    m_distanceToFarPlane{0.0f}
{

}

/**
 * \brief
 */
QMatrix4x4 Camera::viewMatrixAtPosition() const
{
    return viewMatrixAt(m_position);
}

/**
 * \brief
 */
QMatrix4x4 Camera::viewMatrixAtOrigin() const
{
    return viewMatrixAt(ORIGIN);
}

/**
 * \brief
 */
QMatrix4x4 Camera::projectionMatrix(const float aspectRatio) const
{
    QMatrix4x4 projection; // Load the identity matrix
    projection.perspective(m_fov, aspectRatio, m_distanceToNearPlane, m_distanceToFarPlane);

    return projection;
}

/**
 * \brief
 */
void Camera::setPosition(const QVector3D position)
{
    m_position = position;
}

/**
 * \brief
 */
QVector3D Camera::position() const
{
    return m_position;
}

/**
 * \brief
 */
float Camera::fieldOfView() const
{
    return m_fov;
}

/**
 * \brief
 */
void Camera::setFieldOfView(float fieldOfView)
{
    if((fieldOfView >= 0.0f) && (fieldOfView <= 180.0f))
    {
        m_fov = fieldOfView;
    }
}

/**
 * \brief
 */
float Camera::distanceToNearPlane() const
{
    return m_distanceToNearPlane;
}

/**
 * \brief
 */
void Camera::setDistanceToNearPlane(float distance)
{
    if(distance <= m_distanceToFarPlane)
    {
        m_distanceToNearPlane = distance;
    }
}

/**
 * \brief
 */
float Camera::distanceToFarPlane() const
{
    return m_distanceToFarPlane;
}

/**
 * \brief
 */
void Camera::setDistanceToFarPlane(float distance)
{
    if(distance >= m_distanceToNearPlane)
    {
        m_distanceToFarPlane = distance;
    }
}

/**
 * \brief
 */
QMatrix4x4 Camera::viewMatrixAt(const QVector3D& position) const
{
    auto direction = (m_position - ORIGIN).normalized();
    auto right = QVector3D::crossProduct(Y_AXIS, direction).normalized();
    auto up = QVector3D::crossProduct(direction, right);

    QMatrix4x4 view; // Identity matrix is now here
    view.lookAt(position, ORIGIN, up);

    return view;
}

