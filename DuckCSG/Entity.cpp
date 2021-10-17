#include "duckpch.h"
#include "Entity.h"

namespace MapTools {

    Entity::Entity()
    {
    }

    Entity::~Entity()
    {
        DC_CORE_TRACE("Entity destructor called. Bye");
    }

    void Entity::pushProperty(std::string property, std::string value)
    {
        m_properties.push_back({ property, value });
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
