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
    private:
        EntityType m_type;
        std::vector<std::pair<std::string, std::string>> m_properties;
    };
}