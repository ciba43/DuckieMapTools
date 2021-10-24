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
        InBrushExpectingStartOfPlanesOrEnd,
        InBrushDoingPlanes,
    };

    enum class ParserPlaneState
    {
        Outside, 
        DoingPlane1, // Expecting plane 1 is already done by ParserState::InBrushExpectingStartOfPlanesOrEnd
        ExpectingPlane2,
        DoingPlane2,
        ExpectingPlane3,
        DoingPlane3,
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
    private:
        void setAppropriatePlanePoint(Ref<Plane> plane, const PlanePoint& point);
        void incrementPlanePointState();
    private:
        ParserState m_state = ParserState::OutsideEntity;
        ParserPlaneState m_planeState = ParserPlaneState::Outside;
        ParserPointCoordinateState m_pointCoordinateState = ParserPointCoordinateState::Outside;
        std::string m_map;
        bool m_parsed = false;
        std::vector<Ref<Entity>> m_entities;
    };
}
