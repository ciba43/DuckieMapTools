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

    class Entity
    {
    public:
        Entity();
        ~Entity();

        void pushProperty(std::string property, std::string value);

        size_t propertyCount() { return m_properties.size(); }
        std::string findPropertySlow(std::string name);
    private:
        EntityType m_type;
        std::vector<std::pair<std::string, std::string>> m_properties;
    };
}