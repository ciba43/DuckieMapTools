#pragma once

#include <string>
#include <utility> // For std::pair
#include <vector>

#include "Brush.h"

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
        void pushBrush(Ref<Brush> brush);

        size_t propertyCount() { return m_properties.size(); }
        std::string findPropertySlow(std::string name);
    private:
        EntityType m_type;
        std::vector<std::pair<std::string, std::string>> m_properties;
        std::vector<Ref<Brush>> m_brushes;
    };
}