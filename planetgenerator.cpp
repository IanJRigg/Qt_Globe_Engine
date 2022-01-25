#include "planetgenerator.h"

#include <thread>
#include <utility>
#include <QVector3D>

namespace
{
    constexpr auto FRONT_FACE_START_INDEX  = 0UL;
    constexpr auto BACK_FACE_START_INDEX   = 1UL;
    constexpr auto LEFT_FACE_START_INDEX   = 2UL;
    constexpr auto RIGHT_FACE_START_INDEX  = 3UL;
    constexpr auto TOP_FACE_START_INDEX    = 4UL;
    constexpr auto BOTTOM_FACE_START_INDEX = 5UL;
}

/**
 * \brief
 */
static void generate_front_face(std::vector<float>& vertices,
                                std::vector<uint32_t>& indices,
                                const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * FRONT_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(j * step - 0.5f); // X
            temp.setY(i * step - 0.5f); // Y
            temp.setZ(0.5f);            // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n += 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * FRONT_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * FRONT_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
static void generate_back_face(std::vector<float>& vertices,
                               std::vector<uint32_t>& indices,
                               const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * BACK_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(1.0f - (j * step) - 0.5f); // X
            temp.setY(i * step - 0.5f); // Y
            temp.setZ(-0.5f);            // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n+= 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * BACK_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * BACK_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
static void generate_left_face(std::vector<float>& vertices,
                               std::vector<uint32_t>& indices,
                               const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * LEFT_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(-0.5f);           // X
            temp.setY(i * step - 0.5f); // Y
            temp.setZ(j * step - 0.5f); // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n += 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * LEFT_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * LEFT_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
static void generate_right_face(std::vector<float>& vertices,
                                std::vector<uint32_t>& indices,
                                const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * RIGHT_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(0.5f);            // X
            temp.setY(i * step - 0.5f); // Y
            temp.setZ(1.0f - (j * step) - 0.5f); // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n += 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * RIGHT_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * RIGHT_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
static void generate_top_face(std::vector<float>& vertices,
                              std::vector<uint32_t>& indices,
                              const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * TOP_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(1.0f - (j * step) - 0.5f); // X
            temp.setY(0.5f);            // Y
            temp.setZ(i * step - 0.5f); // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n += 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * TOP_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * TOP_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
static void generate_bottom_face(std::vector<float>& vertices,
                                 std::vector<uint32_t>& indices,
                                 const uint32_t vertices_per_side)
{
    float step = 1.0 / (vertices_per_side - 1UL);
    uint32_t n = (vertices_per_side * vertices_per_side * 3) * BOTTOM_FACE_START_INDEX;

    QVector3D temp;

    for(uint32_t i = 0; i < vertices_per_side; i++)
    {
        for(uint32_t j = 0; j < vertices_per_side; j++)
        {
            temp.setX(j * step - 0.5f); // X
            temp.setY(-0.5f);           // Y
            temp.setZ(i * step - 0.5f); // Z

            temp.normalize();

            vertices.at(n + 0UL) = temp.x();
            vertices.at(n + 1UL) = temp.y();
            vertices.at(n + 2UL) = temp.z();

            n += 3UL;
        }
    }

    n = ((vertices_per_side - 1UL)  * (vertices_per_side - 1UL) * 6UL) * BOTTOM_FACE_START_INDEX;
    uint32_t offset = vertices_per_side * vertices_per_side * BOTTOM_FACE_START_INDEX;

    for(uint32_t i = 0; i < vertices_per_side - 1; ++i)
    {
        for(uint32_t j = 0; j < vertices_per_side - 1; ++j)
        {
            indices.at(n + 0UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 1UL) = offset + (i * vertices_per_side) + j + 1UL; // Y
            indices.at(n + 2UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Z

            indices.at(n + 3UL) = offset + (i * vertices_per_side) + j;       // X
            indices.at(n + 4UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 1UL); // Y
            indices.at(n + 5UL) = offset + (i * vertices_per_side) + j + (vertices_per_side + 0UL); // Z

            n += 6UL;
        }
    }
}

/**
 * \brief
 */
std::pair<std::vector<float>, std::vector<uint32_t>>
generateSubdividedCube(const uint32_t numberOfSubdivisions)
{
    const auto verticesPerSide = numberOfSubdivisions + 2;

    std::vector<float> vertices(((verticesPerSide * verticesPerSide * 3) * 6), 0.0f);
    std::vector<uint32_t> indices((((verticesPerSide - 1) * (verticesPerSide - 1) * 6) * 6), 0UL);

    std::thread front_thread(generate_front_face,   std::ref(vertices), std::ref(indices), verticesPerSide);
    std::thread back_thread(generate_back_face,     std::ref(vertices), std::ref(indices), verticesPerSide);
    std::thread left_thread(generate_left_face,     std::ref(vertices), std::ref(indices), verticesPerSide);
    std::thread right_thread(generate_right_face,   std::ref(vertices), std::ref(indices), verticesPerSide);
    std::thread top_thread(generate_top_face,       std::ref(vertices), std::ref(indices), verticesPerSide);
    std::thread bottom_thread(generate_bottom_face, std::ref(vertices), std::ref(indices), verticesPerSide);

    front_thread.join();
    back_thread.join();
    left_thread.join();
    right_thread.join();
    top_thread.join();
    bottom_thread.join();

    return std::make_pair(vertices, indices);
}
