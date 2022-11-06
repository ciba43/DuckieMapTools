#include "duckpch.h"
#include "Polygon.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "MapParser.h"
#include "Plane.h"

namespace MapTools {

    void Polygon::calculateUVsForVertices(const std::vector<TextureInfo>& usedTextures)
    {
        auto plane = m_planeFriend.lock();

        // Find the correct texture info struct
        auto texture = std::find_if(usedTextures.begin(), usedTextures.end(), [&plane](const TextureInfo& texinfo) { return texinfo.name == plane->texture(); });

        // Crash if that somehow doesn't exist (means texture info building process must've failed)
        if (texture == usedTextures.end())
        {
            DC_CORE_CRITICAL("Internal TextureInfo list missing texture. This should never happen");
            VERIFY_NOT_REACHED();
        }

        for (auto& vertex : m_vertices)
        {
            float u = glm::dot(vertex.position(), plane->UVstuff().normalU) / texture->width / plane->scaleU();
            u += plane->UVstuff().shiftU / texture->width;

            float v = glm::dot(vertex.position(), plane->UVstuff().normalV) / texture->height / plane->scaleV();
            v += plane->UVstuff().shiftV / texture->height;
            vertex.setUV({ u, v });
        }
    }

    void Polygon::sortVertices()
    {
        if (!m_vertices.size())
            return;

        // Find center point
        glm::vec3 center{0.0f};
        for (auto& vertex : m_vertices)
        {
            center += vertex.position();
        }
        center /= m_vertices.size();

        auto plane = m_planeFriend.lock();
        for (int i = 0; i < m_vertices.size() - 2; i++)
        {
            glm::vec3 vert_one = glm::normalize(m_vertices[i].position() - center);
            Plane skipPlane = Plane(m_vertices[i].position(), center, plane->normal());

            double smallestAngle = -1;
            int smallestIndex = -1;
            for (int j = i + 1; j < m_vertices.size(); j++)
            {
                if (skipPlane.classifyPoint(m_vertices[j].position()) != PointType::InBack) {
                    glm::vec3 vert_two = glm::normalize(m_vertices[j].position() - center);
                    float angle = glm::dot(vert_one, vert_two);
                    if (angle > smallestAngle) {
                        smallestAngle = angle;
                        smallestIndex = j;
                    }
                }
            }

            // This should never fail
            if (smallestAngle == -1) {
                DC_CORE_CRITICAL("Polygon sorting failed, idk how");
                return;
            }

            // Swap them around
            auto oldSmallest = m_vertices[smallestIndex];
            m_vertices[smallestIndex] = m_vertices[i + 1];
            m_vertices[i + 1] = oldSmallest;
        }

    }

    bool Polygon::hasVertex(const glm::vec3& vertex)
    {
        for (auto& vert : m_vertices)
        {
            if (glm::all(glm::epsilonEqual(vertex, vert.position(), 0.01f)))
                return true;
        }
        return false;
    }
}
