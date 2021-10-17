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

    private:
        ParserState m_state = ParserState::OutsideEntity;
        std::string m_map;
        bool m_parsed = false;
        std::vector<Ref<Entity>> m_entities;
    };
}
