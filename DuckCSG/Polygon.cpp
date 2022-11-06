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

        glm::vec3 center{0.0f};

        // Find center point
        for (auto& vertex : m_vertices)
        {
            center += vertex.position();
        }
        center /= m_vertices.size();

        DC_CORE_INFO("{0},  {1},  {2}", center.x, center.y, center.z);

        DC_CORE_TRACE("normalized");
        for (int i = 0; i < m_vertices.size() - 2; i++)
        {
            glm::vec3 vert_one = glm::normalize(m_vertices[i].position() - center);
            DC_CORE_TRACE("vert_one: {0}  {1}  {2}", vert_one.x, vert_one.y, vert_one.z);

            double smallestAngle = -1;
            int smallestIndex = -1;
            for (int j = i + 1; j < m_vertices.size(); j++)
            {
                /* code */
            }

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
