#include "duckpch.h"
#include "Plane.h"

namespace MapTools {

    Plane::Plane()
    {
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
    }

    void Plane::setTextureString(std::string texture)
    {
        m_textureName = texture;
    }

    void Plane::setScaleU(float scaleU)
    {
        m_scaleU = scaleU;
    }

    void Plane::setScaleV(float scaleV)
    {
        m_scaleV = scaleV;
    }
}
