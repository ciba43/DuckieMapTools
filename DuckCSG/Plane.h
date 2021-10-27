#pragma once

#include <string>

#include <glm/vec3.hpp>

namespace MapTools {

    struct PlanePoint
    {
        float x;
        float y;
        float z;
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
        Plane();

        void setPoint1(PlanePoint point);
        void setPoint2(PlanePoint point);
        void setPoint3(PlanePoint point);

        void setTextureString(std::string texture);

        UV& UVstuff() { return m_uv; }

        void setScaleU(float scaleU);
        void setScaleV(float scaleV);
    private:
        PlanePoint m_point1;
        PlanePoint m_point2;
        PlanePoint m_point3;
        UV m_uv;
        std::string m_textureName;
        float m_scaleU;
        float m_scaleV;
    };
}
