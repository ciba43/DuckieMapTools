#pragma once

#include <string>
#include <vector>

#include "Core.h"

namespace MapTools {

    class Brush;
    class Entity;
    class Plane;
    struct PlanePoint;

    struct TextureInfo
    {
        std::string name;
        int width = 4;  // Default Duckie error texture is 4x4
        int height = 4;
        bool missing = true;
    };

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
        InBrushExpectingU,
        InBrushReadingU,
        InBrushExpectingV,
        InBrushReadingV,
        InBrushReadingTextureRotation,
        InBrushReadingUScale,
        InBrushReadingVScale
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

    enum class ParserUVState
    {
        Outside,
        DoingNormalX,
        DoingNormalY,
        DoingNormalZ,
        DoingShift
    };

    class MapParser
    {
    public:
        MapParser(std::string map);

        // Do the actual parsing
        ParseReturnState parse();

        const std::string& mapName() { return m_map; }

        // Get all of the parsed properties
        size_t entityCount() { return m_entities.size(); }
        Ref<Entity> entity(unsigned int handle);

        void createPolygonsFromBrushes();
        void generateListOfAllUsedTextures();
        void acquireDimensionsForUsedTextures();
        void calculateTextureCoordinates();
        void sortVertices();

        static float parseFloat(const std::string& string);
        static bool isValidTextureNameCharacter(char character);
    private:
        void setAppropriatePlanePoint(Ref<Plane> plane, const PlanePoint& point);
        void incrementPlanePointState();
        void incrementUVState();
    private:
        ParserState m_state = ParserState::OutsideEntity;
        ParserPointState m_pointState = ParserPointState::Outside;
        ParserPointCoordinateState m_pointCoordinateState = ParserPointCoordinateState::Outside;
        ParserUVState m_parserUVstate = ParserUVState::Outside;
        std::string m_map;
        bool m_parsed = false;
        std::vector<Ref<Entity>> m_entities;

        std::vector<TextureInfo> m_usedTextures;
    };
}
