#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace MapTools {

    class Polygon
    {
    public:
        Polygon() {}

        void addVertex(glm::vec3 vertex) { m_vertices.push_back(vertex); }
    private:
        std::vector<glm::vec3> m_vertices;
    };

}