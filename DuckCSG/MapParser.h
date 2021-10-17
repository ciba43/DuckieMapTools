#pragma once

#include <string>
#include <vector>

#include "Core.h"

namespace MapTools {

    class Entity;

    enum class ParseReturnState
    {
        OK,
        MissingFile
    };

    enum class ParserState
    {
        OutsideEntity,
        ExpectingPropertyOrStartOfBrush,
        InPropertyName,
        ExpectingPropertyValue,
        InPropertyValue,
        InBrushAndExpectingPlaneOrEnd,
        InPlane,
    };

    class MapParser
    {
    public:
        MapParser(std::string map);

        // Do the actual parsing
        ParseReturnState parse();

        // Get all of the parsed properties
        size_t entityCount() { return m_entities.size(); }
        Ref<Entity> entity(unsigned int handle);

    private:
        ParserState m_state = ParserState::OutsideEntity;
        std::string m_map;
        bool m_parsed = false;
        std::vector<Ref<Entity>> m_entities;
    };
}
