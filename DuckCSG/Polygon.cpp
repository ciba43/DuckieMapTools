#include "duckpch.h"
#include "Polygon.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Plane.h"

namespace MapTools {

    bool Polygon::hasVertex(const glm::vec3 & vertex)
    {
        for (auto& vert : m_vertices)
        {
            if (glm::all(glm::epsilonEqual(vertex, vert.position(), 0.01f)))
                return true;
        }
        return false;
    }
}
