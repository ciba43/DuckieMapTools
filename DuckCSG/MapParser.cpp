#include "duckpch.h"
#include "MapParser.h"

#include "Brush.h"
#include "Entity.h"

namespace MapTools {

    MapParser::MapParser(std::string map)
    {
        m_map = map;
    }

    ParseReturnState MapParser::parse()
    {
        // Log if we've already parsed the file
        if (m_parsed)
            DC_CORE_INFO("[Parser] Already parsed but reparsing requested");
        m_parsed = true;

        // Load file
        std::ifstream mapFile;
        mapFile.open(m_map, std::ios::binary);

        if (!mapFile.is_open())
            return ParseReturnState::MissingFile;

        std::string temporaryPropertyName;
        std::string temporaryPropertyValue;

        Ref<Entity> currentEntity;

        Ref<Brush> temporaryBrush;
        std::string temporaryPointCoordinateString;
        PlanePoint temporaryPoint;

        char readCharacter;
        while (!mapFile.eof())
        {
            mapFile.read(&readCharacter, 1);

            switch (m_state)
            {
            case ParserState::OutsideEntity:
            {
                // CR and LF
                if (readCharacter == 0x0A || readCharacter == 0x0D)
                    continue;

                // We're starting a new entity
                if (readCharacter == '{')
                {
                    // Create a new entity
                    currentEntity = createRef<Entity>();

                    DC_CORE_TRACE("[Parser] Entering ParserState::ExpectingPropertyOrStartOfBrush");
                    m_state = ParserState::ExpectingPropertyOrStartOfBrush;
                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::ExpectingPropertyOrStartOfBrush:
            {
                // CR and LF
                if (readCharacter == 0x0A || readCharacter == 0x0D)
                    continue;

                if (readCharacter == '"')
                {
                    DC_CORE_TRACE("[Parser] Entering ParserState::InPropertyName");
                    m_state = ParserState::InPropertyName;
                    continue;
                }

                if (readCharacter == '{')
                {
                    DC_CORE_TRACE("[Parser] Entering ParserState::InBrushAndExpectingPlaneOrEnd");
                    m_state = ParserState::InBrushExpectingStartOfPlanesOrEnd;
                    continue;
                }

                // End of entity
                if (readCharacter == '}')
                {
                    // Push the entity reference
                    m_entities.push_back(currentEntity);
                    currentEntity.reset();

                    DC_CORE_TRACE("[Parser] Entering ParserState::OutsideEntity");
                    m_state = ParserState::OutsideEntity;
                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InPropertyName:
            {
                if (readCharacter == '"')
                {
                    DC_CORE_TRACE("[Parser] Entering ParserState::ExpectingPropertyValue");
                    m_state = ParserState::ExpectingPropertyValue;
                    continue;
                }
                temporaryPropertyName.push_back(readCharacter);
                continue;
            }
            case ParserState::ExpectingPropertyValue:
            {
                if (readCharacter == '"')
                {
                    DC_CORE_TRACE("[Parser] Entering ParserState::InPropertyValue");
                    m_state = ParserState::InPropertyValue;
                    continue;
                }

                // Property names and values are separated with an empty space
                if (readCharacter == ' ')
                    continue;

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InPropertyValue:
            {
                if (readCharacter == '"')
                {
                    // Save the property and clear the temporary variables
                    currentEntity->pushProperty(temporaryPropertyName, temporaryPropertyValue);
                    temporaryPropertyName.clear();
                    temporaryPropertyValue.clear();

                    DC_CORE_TRACE("[Parser] Entering ParserState::ExpectingPropertyOrStartOfBrush");
                    m_state = ParserState::ExpectingPropertyOrStartOfBrush;
                    continue;
                }
                temporaryPropertyValue.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushExpectingStartOfPlanesOrEnd:
            {
                // CR and LF
                //if (readCharacter == 0x0A || readCharacter == 0x0D)
                //continue;

                // End of brush
                if (readCharacter == '}')
                {
                    DC_CORE_TRACE("[Parser] Entering ParserState::ExpectingPropertyOrStartOfBrush");
                    m_state = ParserState::ExpectingPropertyOrStartOfBrush;
                    continue;
                }

                // Start of the first plane point description
                if (readCharacter == '(')
                {
                    // Create a new Brush
                    temporaryBrush = createRef<Brush>();

                    DC_CORE_TRACE("[Parser] Entering ParserState::InBrushDoingPlanes");
                    m_state = ParserState::InBrushDoingPlanes;
                    m_planeState = ParserPlaneState::DoingPlane1;
                    m_pointCoordinateState = ParserPointCoordinateState::DoingPointX;
                    temporaryPointCoordinateString.clear();

                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushDoingPlanes:
            {
                if (readCharacter == ' ')
                {
                    // Ignore spaces if we haven't read any data yet
                    if (temporaryPointCoordinateString.empty())
                        continue;

                    // Otherwise we jump to the next coordinate
                    switch (m_pointCoordinateState)
                    {
                    case ParserPointCoordinateState::DoingPointX:
                        temporaryPoint.x = parseFloat(temporaryPointCoordinateString);
                        temporaryPointCoordinateString.clear();
                        m_pointCoordinateState = ParserPointCoordinateState::DoingPointY;
                        continue;

                    case ParserPointCoordinateState::DoingPointY:

                        temporaryPoint.y = parseFloat(temporaryPointCoordinateString);
                        temporaryPointCoordinateString.clear();
                        m_pointCoordinateState = ParserPointCoordinateState::DoingPointZ;
                        continue;

                    case ParserPointCoordinateState::DoingPointZ: // End of this point
                        temporaryPoint.z = parseFloat(temporaryPointCoordinateString);
                        temporaryPointCoordinateString.clear();

                        temporaryBrush->setPoint1(temporaryPoint);
                        m_pointCoordinateState = ParserPointCoordinateState::Outside;

                        continue;
                    case ParserPointCoordinateState::Outside:
                    default:
                        VERIFY_NOT_REACHED();
                        break;
                    }
                }

                temporaryPointCoordinateString.push_back(readCharacter);
                continue;
            }
            default:
                TODO();
                break;
            }

            DC_CORE_TRACE(readCharacter);
        }

        mapFile.close();

        return ParseReturnState::OK;
    }

    Ref<Entity> MapParser::entity(unsigned int handle)
    {
        if (handle < m_entities.size())
            return m_entities.at(handle);

        return {};
    }

    float MapParser::parseFloat(const std::string & string)
    {
        // TODO:
        return 0.0f;
    }
}
