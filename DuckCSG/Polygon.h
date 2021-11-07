#pragma once

#include <glm/gtc/epsilon.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace MapTools {

    class Polygon
    {
    public:
        Polygon() {}

        void addVertex(glm::vec3 vertex) { m_vertices.push_back(vertex); }
        size_t vertexCount() { return m_vertices.size(); }
        bool hasVertex(const glm::vec3& vertex)
        {
            for (auto& vert : m_vertices)
            {
                if (glm::all(glm::epsilonEqual(vertex, vert, 0.01f)))
                    return true;
            }
            return false;
        }
    private:
        std::vector<glm::vec3> m_vertices;
    };

}