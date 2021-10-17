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
}
