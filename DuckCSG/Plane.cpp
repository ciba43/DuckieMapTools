#include "duckpch.h"
#include "Plane.h"

#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>

namespace MapTools {

    Plane::Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
    {
        setPoint1(p1);
        setPoint2(p2);
        setPoint3(p3);
    }

    void Plane::setPoint1(PlanePoint point)
    {
        m_point1 = point;
    }

    void Plane::setPoint2(PlanePoint point)
    {
        m_point2 = point;
    }

    void Plane::setPoint3(PlanePoint point)
    {
        m_point3 = point;

        // Calculate the plane normal and distance
        glm::vec3 p1 = { m_point1.x, m_point1.y, m_point1.z };
        glm::vec3 p2 = { m_point2.x, m_point2.y, m_point2.z };
        glm::vec3 p3 = { m_point3.x, m_point3.y, m_point3.z };

        // I don't know what I'm doing 
        m_normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));
        m_distance = -glm::dot(m_normal, p3);
    }

    void Plane::setTextureString(std::string texture)
    {
        m_textureName = texture;
    }

    void Plane::setTextureRotation(float rotation)
    {
        m_rotation = rotation;
        m_isRotationSet = true;
    }

    void Plane::setScaleU(float scaleU)
    {
        m_scaleU = scaleU;
    }

    void Plane::setScaleV(float scaleV)
    {
        m_scaleV = scaleV;
    }

    PointType Plane::classifyPoint(const glm::vec3& point)
    {
        double distance = glm::dot(m_normal, point) + m_distance;
        if (distance > glm::epsilon<double>())
            return PointType::InFront;
        else if (distance < -glm::epsilon<double>())
            return PointType::InBack;

        return PointType::OnPlane;
    }
}
