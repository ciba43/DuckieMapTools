#pragma once

#include <string>

#include <glm/vec3.hpp>

namespace MapTools {

    struct PlanePoint
    {
        float x;
        float y;
        float z;

        PlanePoint() {}
        PlanePoint(glm::vec3 pos) : x(pos.x), y(pos.y), z(pos.z) {}
    };

    enum class PointType
    {
        InFront,
        OnPlane,
        InBack
    };

    struct UV
    {
        glm::vec3 normalU;
        float shiftU;

        glm::vec3 normalV;
        float shiftV;
    };

    class Plane
    {
    public:
        Plane() {}
        Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

        void setPoint1(PlanePoint point);
        void setPoint2(PlanePoint point);
        void setPoint3(PlanePoint point);

        PlanePoint& point1() { return m_point1; }
        PlanePoint& point2() { return m_point2; }
        PlanePoint& point3() { return m_point3; }

        const glm::vec3& normal() { return m_normal; }
        const float& distance() { return m_distance; }

        const std::string& texture() { return m_textureName; }

        void setTextureString(std::string texture);

        UV& UVstuff() { return m_uv; }

        void setTextureRotation(float rotation);

        void setScaleU(float scaleU);
        void setScaleV(float scaleV);
        float& scaleU() { return m_scaleU; }
        float& scaleV() { return m_scaleV; }

        PointType classifyPoint(const glm::vec3& point);
    private:
        glm::vec3 m_normal;
        float m_distance;

        PlanePoint m_point1;
        PlanePoint m_point2;
        PlanePoint m_point3;
        std::string m_textureName;
        UV m_uv;
        float m_rotation = 0.0f;
        bool m_isRotationSet = false;
        float m_scaleU;
        float m_scaleV;
    };
}
