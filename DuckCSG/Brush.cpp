#include "duckpch.h"
#include "Brush.h"

#include "Plane.h"
#include "Polygon.h"

#include <glm/geometric.hpp>

namespace MapTools {

    Brush::Brush()
    {
    }

    void Brush::pushPlane(Ref<Plane> plane)
    {
        m_planes.push_back(plane);
    }

    void Brush::createPolygons()
    {
        for (size_t i = 0; i < m_planes.size() - 1; i++)
        {
            for (size_t j = 0; j < m_planes.size() - 1; j++)
            {
                for (size_t k = 0; k < m_planes.size() - 1; k++)
                {
                    if (i != j != k)
                    {
                        // Create the polygon if it doesn't exist
                        if (i + 1 > m_polygons.size())
                            m_polygons.push_back(createRef<Polygon>());
                        getIntersection(i, j, k);
                        m_polygons.at(i)->addVertex();
                    }
                }
            }
        }
    }

    std::pair<bool, glm::vec3> Brush::getIntersection(size_t plane1, size_t plane2, size_t plane3)
    {
        if (plane1 > m_planes.size() || plane2 > m_planes.size() || plane3 > m_planes.size())
        {
            DC_CORE_CRITICAL("Plane id out of bounds {0} > {1}", plane1, m_planes.size());
            return { false, glm::vec3() };
        }

        auto firstPlane = m_planes.at(plane1);
        auto secondPlane = m_planes.at(plane2);
        auto thirdPlane = m_planes.at(plane3);

        float denominator = glm::dot(firstPlane->normal(), glm::cross(secondPlane->normal(),thirdPlane->normal()));

        // Is there no intersection (planes are parallel)?
        if (denominator == 0)
            return { false, glm::vec3() };

        glm::vec3 point = -firstPlane->distance() * glm::cross(secondPlane->normal(), thirdPlane->normal()) - secondPlane->distance() * glm::cross(thirdPlane->normal(), firstPlane->normal()) - thirdPlane->distance() * glm::cross(firstPlane->normal(), secondPlane->normal()) / denominator;

        return { true, point };
    }

    bool Brush::isValid()
    {
        // TODO:
        return false;
    }
}
