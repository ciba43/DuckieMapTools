#pragma once

#include <string>
#include <vector>

#include "Core.h"

namespace MapTools {

    class Brush;
    class Entity;
    class Plane;
    struct PlanePoint;

    enum class ParseReturnState
    {
        OK,
        MissingFile
    };

    enum class ParserState
    {
        OutsideEntity,
        ExpectingPropertyOrStartOfBrush,

        // Property stuff
        InPropertyName,
        ExpectingPropertyValue,
        InPropertyValue,

        // Brush stuff
        InBrushExpectingPlaneOrEnd,
        InBrushDoingPlanePoints,
        InBrushExpectingPlaneTexture,
        InBrushReadingPlaneTexture,
    };

    enum class ParserPointState
    {
        Outside, 
        DoingPoint1, // Expecting point 1 is already done by ParserState::InBrushExpectingPlaneOrEnd
        ExpectingPoint2,
        DoingPoint2,
        ExpectingPoint3,
        DoingPoint3,
    };

    enum class ParserPointCoordinateState
    {
        Outside,
        DoingPointX,
        DoingPointY,
        DoingPointZ
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

        static float parseFloat(const std::string& string);
        static bool isValidTextureNameCharacter(char character);
    private:
        void setAppropriatePlanePoint(Ref<Plane> plane, const PlanePoint& point);
        void incrementPlanePointState();
    private:
        ParserState m_state = ParserState::OutsideEntity;
        ParserPointState m_pointState = ParserPointState::Outside;
        ParserPointCoordinateState m_pointCoordinateState = ParserPointCoordinateState::Outside;
        std::string m_map;
        bool m_parsed = false;
        std::vector<Ref<Entity>> m_entities;
    };
}
