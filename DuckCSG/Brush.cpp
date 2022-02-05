#include "duckpch.h"
#include "Brush.h"

#include "Plane.h"
#include "Polygon.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

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
        for (size_t i = 0; i < m_planes.size(); i++)
        {
            for (size_t j = 0; j < m_planes.size(); j++)
            {
                for (size_t k = 0; k < m_planes.size(); k++)
                {
                    if (i != j != k)
                    {
                        // Create the polygon if it doesn't exist
                        if (i + 1 > m_polygons.size())
                            m_polygons.push_back(createRef<Polygon>());
                        auto intersection = getIntersection(i, j, k);

                        // Skip if planes were parallel
                        if (!intersection.first)
                            continue;

                        // Make sure the point isn't outside the brush
                        bool illegalVertex = false;
                        for (size_t m = 0; m < m_planes.size(); m++)
                        {
                            auto dot = glm::dot(m_planes.at(m)->normal(), intersection.second);
                            auto dist = m_planes.at(m)->distance();

                            if (glm::epsilonEqual(dot + dist, 0.0f, 0.01f) || dot + dist < 0)
                                continue;

                            illegalVertex = true;
                        }

                        // Skip illegal vertices
                        if (illegalVertex)
                            continue;

                        // Skip existing vertices
                        if (isVertexInPolygon(i, intersection.second))
                            continue;

                        m_polygons.at(i)->addVertex(intersection.second);
                    }
                }
            }
        }
    }

    std::vector<std::string> Brush::generateListOfAllUsedTextures()
    {
        std::vector<std::string> textures;

        for (auto& plane : m_planes)
        {
            bool wasTextureFound = false;
            for (auto& texture : textures)
            {
                if (texture == plane->texture())
                    wasTextureFound = true;
            }
            if (!wasTextureFound)
                textures.push_back(plane->texture());
        }

        return textures;
    }

    void Brush::calculateTextureCoordinates()
    {
        for (size_t i = 0; i < m_polygons.size(); i++)
        {
            m_polygons.at(i)->calculateUVsForVertices(m_planes.at(i));
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

        glm::vec3 point = (-firstPlane->distance() * glm::cross(secondPlane->normal(), thirdPlane->normal()) - secondPlane->distance() * glm::cross(thirdPlane->normal(), firstPlane->normal()) - thirdPlane->distance() * glm::cross(firstPlane->normal(), secondPlane->normal())) / denominator;

        return { true, point };
    }

    bool Brush::isValid()
    {
        // TODO:
        return false;
    }

    bool Brush::isVertexInPolygon(size_t polygon, const glm::vec3& vertex)
    {
        return m_polygons.at(polygon)->hasVertex(vertex);
    }
}
