#include "camera.h"

namespace
{
    QVector3D ORIGIN { 0.0f, 0.0f, 0.0f };
    QVector3D Y_AXIS { 0.0f, 1.0f, 0.0f };

    constexpr auto FIELD_OF_VIEW_LOWER_LIMIT = 0.0f;
    constexpr auto FIELD_OF_VIEW_UPPER_LIMIT = 180.0f;

    constexpr auto DEFAULT_NEAR_PLANE_DISTANCE = 0.0f;
    constexpr auto DEFAULT_FAR_PLANE_DISTANCE = 0.0f;
}

/**
 * \brief Constructor for the Camera class. Input is the cartesian position
 */
Camera::Camera(const float x, const float y, const float z) :
    m_position(x, y, z),
    m_fov{FIELD_OF_VIEW_LOWER_LIMIT},
    m_distanceToNearPlane{DEFAULT_NEAR_PLANE_DISTANCE},
    m_distanceToFarPlane{DEFAULT_FAR_PLANE_DISTANCE}
{

}

/**
 * \brief Calculates the view matrix using the current position
 */
QMatrix4x4 Camera::viewMatrixAtPosition() const
{
    return viewMatrixAt(m_position);
}

/**
 * \brief Calculates the view matrix using the origin as the current position
 */
QMatrix4x4 Camera::viewMatrixAtOrigin() const
{
    return viewMatrixAt(ORIGIN);
}

/**
 * \brief Creates the projection matrix. This is implemented as a perspective matrix.
 *        In the future, this could change to an orthogonal matrix.
 */
QMatrix4x4 Camera::projectionMatrix(const float aspectRatio) const
{
    QMatrix4x4 projection; // Load the identity matrix
    projection.perspective(m_fov, aspectRatio, m_distanceToNearPlane, m_distanceToFarPlane);

    return projection;
}

/**
 * \brief Accessor for the camera position
 */
QVector3D Camera::position() const
{
    return m_position;
}

/**
 * \brief Mutator for the camera position
 */
void Camera::setPosition(const QVector3D& position)
{
    m_position = position;
}

/**
 * \brief Accessor for the field of view
 */
float Camera::fieldOfView() const
{
    return m_fov;
}

/**
 * \brief Mutator for the field of view
 */
void Camera::setFieldOfView(const float fieldOfView)
{
    if((fieldOfView >= FIELD_OF_VIEW_LOWER_LIMIT) && (fieldOfView <= FIELD_OF_VIEW_UPPER_LIMIT))
    {
        m_fov = fieldOfView;
    }
}

/**
 * \brief Accessor for the near plane distance
 */
float Camera::distanceToNearPlane() const
{
    return m_distanceToNearPlane;
}

/**
 * \brief Mutator for the near plane distance
 */
void Camera::setDistanceToNearPlane(const float distance)
{
    if(distance <= m_distanceToFarPlane)
    {
        m_distanceToNearPlane = distance;
    }
}

/**
 * \brief Accessor for the far plane distance
 */
float Camera::distanceToFarPlane() const
{
    return m_distanceToFarPlane;
}

/**
 * \brief Mutator for the far plane distance
 */
void Camera::setDistanceToFarPlane(const float distance)
{
    if(distance >= m_distanceToNearPlane)
    {
        m_distanceToFarPlane = distance;
    }
}

/**
 * \brief Utility function to calculate the view matrix at the provided position
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

