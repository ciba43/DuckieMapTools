#pragma once

#include <glm/gtc/epsilon.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace MapTools {

    class Vertex
    {
    public:
        Vertex(glm::vec3 position) { m_position = position; }

        const glm::vec3& position() { return m_position; }
        const glm::vec3& UV() { return m_UV; }
        void setUV(glm::vec3 uv) { m_UV = uv; }
    private:
        glm::vec3 m_position;
        glm::vec3 m_UV;
    };

    class Polygon
    {
    public:
        Polygon() {}

        void addVertex(glm::vec3 vertex) { m_vertices.push_back(Vertex(vertex)); }

        size_t vertexCount() { return m_vertices.size(); }
        bool hasVertex(const glm::vec3& vertex);
    private:
        std::vector<Vertex> m_vertices;
    };

}