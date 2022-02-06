#include "duckpch.h"
#include "Entity.h"

#include "Brush.h"

namespace MapTools {

    Entity::Entity()
    {
    }

    Entity::~Entity()
    {
    }

    void Entity::pushProperty(std::string property, std::string value)
    {
        m_properties.push_back({ property, value });
    }

    void Entity::pushBrush(Ref<Brush> brush)
    {
        m_brushes.push_back(brush);
    }

    void Entity::createPolygonsFromBrushes()
    {
        for (auto& brush : m_brushes)
        {
            brush->createPolygons();
        }
    }

    std::vector<std::string> Entity::generateListOfAllUsedTextures()
    {
        std::vector<std::string> textures;

        for (auto& brush : m_brushes)
        {
            auto texturesUsed = brush->generateListOfAllUsedTextures();
            for (auto& texture : texturesUsed)
            {
                auto iterator = std::find(textures.begin(), textures.end(), texture);
                if (iterator == textures.end())
                    textures.push_back(texture);
            }
        }

        return textures;
    }

    void Entity::calculateTextureCoordinates(const std::vector<TextureInfo>& usedTextures)
    {
        for (auto& brush : m_brushes)
        {
            brush->calculateTextureCoordinates(usedTextures);
        }
    }

    std::string Entity::findPropertySlow(std::string name)
    {
        auto iterator = std::find_if(m_properties.begin(), m_properties.end(), [&](const std::pair<std::string, std::string>& entry) {
            return entry.first == name;
        });

        if (iterator != m_properties.end())
            return iterator->second;

        return "";
    }
}
