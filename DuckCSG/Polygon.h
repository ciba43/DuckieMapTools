#pragma once

#include <glm/gtc/epsilon.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace MapTools {

    class Plane;

    struct TextureInfo;

    class Vertex
    {
    public:
        Vertex(glm::vec3 position) { m_position = position; }

        const glm::vec3& position() { return m_position; }
        const glm::vec2& UV() { return m_UV; }
        void setUV(glm::vec2 uv) { m_UV = uv; }
    private:
        glm::vec3 m_position;
        glm::vec2 m_UV;
    };

    class Polygon
    {
    public:
        Polygon(WeakRef<Plane> planeFriend) { m_planeFriend = planeFriend; }

        void addVertex(glm::vec3 vertex) { m_vertices.push_back(Vertex(vertex)); }

        void calculateUVsForVertices(const std::vector<TextureInfo>& usedTextures);
        size_t vertexCount() { return m_vertices.size(); }
        bool hasVertex(const glm::vec3& vertex);
    private:
        std::vector<Vertex> m_vertices;
        WeakRef<Plane> m_planeFriend;
    };
}
