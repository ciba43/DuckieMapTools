#pragma once

#include <string>
#include <utility> // For std::pair
#include <vector>

namespace MapTools {

    enum class EntityType
    {
        Point,
        Brush
    };

    class Brush;

    struct TextureInfo;

    class Entity
    {
    public:
        Entity();
        ~Entity();

        void pushProperty(std::string property, std::string value);
        void pushBrush(Ref<Brush> brush);

        void createPolygonsFromBrushes();
        std::vector<std::string> generateListOfAllUsedTextures();
        void calculateTextureCoordinates(const std::vector<TextureInfo>& usedTextures);
        void sortVertices();

        size_t propertyCount() { return m_properties.size(); }
        size_t brushCount() { return m_brushes.size(); }
        std::string findPropertySlow(std::string name);
        Ref<Brush> brush(size_t index) { return m_brushes.at(index); }
    private:
        EntityType m_type;
        std::vector<std::pair<std::string, std::string>> m_properties;
        std::vector<Ref<Brush>> m_brushes;
    };
}