#include "duckpch.h"
#include "MapParser.h"

#include <stb_image.h>

#include "Brush.h"
#include "Entity.h"
#include "Plane.h"

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
        Ref<Plane> temporaryPlane;
        std::string temporaryPointCoordinateString;
        PlanePoint temporaryPoint;

        std::string temporaryTextureName;

        std::string temporaryUVvalue;
        std::string temporaryRotationOrUVScale;

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
                    m_state = ParserState::InPropertyName;
                    continue;
                }

                if (readCharacter == '{')
                {
                    m_state = ParserState::InBrushExpectingPlaneOrEnd;

                    // Create a new Brush
                    temporaryBrush = createRef<Brush>();

                    continue;
                }

                // End of entity
                if (readCharacter == '}')
                {
                    // Push the entity reference
                    m_entities.push_back(currentEntity);
                    currentEntity.reset();

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

                    m_state = ParserState::ExpectingPropertyOrStartOfBrush;
                    continue;
                }
                temporaryPropertyValue.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushExpectingPlaneOrEnd:
            {
                // CR and LF
                if (readCharacter == 0x0A || readCharacter == 0x0D)
                    continue;

                // End of brush
                if (readCharacter == '}')
                {
                    currentEntity->pushBrush(temporaryBrush);
                    m_state = ParserState::ExpectingPropertyOrStartOfBrush;
                    continue;
                }

                // Start of the first plane point description
                if (readCharacter == '(')
                {
                    m_state = ParserState::InBrushDoingPlanePoints;
                    m_pointState = ParserPointState::DoingPoint1;
                    m_pointCoordinateState = ParserPointCoordinateState::DoingPointX;

                    // Create a new plane
                    temporaryPlane = createRef<Plane>();

                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushDoingPlanePoints:
            {
                if (m_pointState == ParserPointState::ExpectingPoint2 || m_pointState == ParserPointState::ExpectingPoint3)
                {
                    // End of current plane point
                    if (readCharacter == ')')
                        continue;

                    // Start of next plane point
                    if (readCharacter == '(')
                    {
                        incrementPlanePointState();
                        m_pointCoordinateState = ParserPointCoordinateState::DoingPointX;
                        continue;
                    }
                }

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

                    case ParserPointCoordinateState::DoingPointZ: // End of this point and maybe the end of ParserState::InBrushDoingPlanePoints
                        temporaryPoint.z = parseFloat(temporaryPointCoordinateString);
                        temporaryPointCoordinateString.clear();

                        setAppropriatePlanePoint(temporaryPlane, temporaryPoint);
                        incrementPlanePointState();

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
            case ParserState::InBrushExpectingPlaneTexture:
            {
                // Handle end of last plane point
                if (readCharacter == ')')
                {
                    m_state = ParserState::InBrushReadingPlaneTexture;
                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushReadingPlaneTexture:
            {
                // End ..
                if (readCharacter == ' ')
                {
                    // .. but not if we haven't started reading the texture name
                    if (temporaryTextureName.empty())
                        continue;

                    // End
                    temporaryPlane->setTextureString(temporaryTextureName);
                    temporaryTextureName.clear();

                    m_state = ParserState::InBrushExpectingU;
                    continue;
                }

                if (isValidTextureNameCharacter(readCharacter))
                {
                    temporaryTextureName.push_back(readCharacter);
                    continue;
                }
                else
                {
                    DC_CORE_ERROR("[Parser] Illegal texture character hit : {0}", readCharacter);

                    VERIFY_NOT_REACHED();
                    break;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushExpectingU:
            {
                // Start U component parsing
                if (readCharacter == '[')
                {
                    m_state = ParserState::InBrushReadingU;
                    m_parserUVstate = ParserUVState::DoingNormalX;
                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushReadingU:
            {
                // Exit
                if (readCharacter == ']')
                {
                    m_state = ParserState::InBrushExpectingV;
                    continue;
                }

                if (readCharacter == ' ')
                {
                    if (temporaryUVvalue.empty())
                        continue;

                    switch (m_parserUVstate)
                    {
                    case ParserUVState::DoingNormalX:
                        temporaryPlane->UVstuff().normalU.x = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingNormalY:
                        temporaryPlane->UVstuff().normalU.y = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingNormalZ:
                        temporaryPlane->UVstuff().normalU.z = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingShift:
                        temporaryPlane->UVstuff().shiftU = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    default:
                        VERIFY_NOT_REACHED();
                        break;
                    }
                }

                temporaryUVvalue.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushExpectingV:
            {
                if (readCharacter == ' ')
                    continue;

                // Start V component parsing
                if (readCharacter == '[')
                {
                    m_state = ParserState::InBrushReadingV;
                    m_parserUVstate = ParserUVState::DoingNormalX;
                    continue;
                }

                VERIFY_NOT_REACHED();
                break;
            }
            case ParserState::InBrushReadingV:
            {
                // Exit
                if (readCharacter == ']')
                {
                    m_state = ParserState::InBrushReadingTextureRotation;
                    continue;
                }

                if (readCharacter == ' ')
                {
                    if (temporaryUVvalue.empty())
                        continue;

                    switch (m_parserUVstate)
                    {
                    case ParserUVState::DoingNormalX:
                        temporaryPlane->UVstuff().normalV.x = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingNormalY:
                        temporaryPlane->UVstuff().normalV.y = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingNormalZ:
                        temporaryPlane->UVstuff().normalV.z = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    case ParserUVState::DoingShift:
                        temporaryPlane->UVstuff().shiftV = parseFloat(temporaryUVvalue);
                        incrementUVState();
                        temporaryUVvalue.clear();
                        continue;

                    default:
                        VERIFY_NOT_REACHED();
                        break;
                    }
                }

                temporaryUVvalue.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushReadingTextureRotation:
            {
                if (readCharacter == ' ')
                {
                    if (temporaryRotationOrUVScale.empty())
                        continue;

                    // Texture rotation shouldn't be used so it should be 0 in most cases, but do notify the user if it is used/set
                    if (temporaryRotationOrUVScale != "0")
                    {
                        DC_CORE_TRACE("[Parser] Setting texture rotation shouldn't be done - encode in UVs please");
                        temporaryPlane->setTextureRotation(parseFloat(temporaryRotationOrUVScale));
                    }

                    temporaryRotationOrUVScale.clear();
                    m_state = ParserState::InBrushReadingUScale;
                    continue;
                }

                temporaryRotationOrUVScale.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushReadingUScale:
            {
                if (readCharacter == ' ')
                {
                    if (temporaryRotationOrUVScale.empty())
                        continue;

                    temporaryPlane->setScaleU(parseFloat(temporaryRotationOrUVScale));
                    temporaryRotationOrUVScale.clear();
                    m_state = ParserState::InBrushReadingVScale;
                    continue;
                }

                temporaryRotationOrUVScale.push_back(readCharacter);
                continue;
            }
            case ParserState::InBrushReadingVScale:
            {
                // CR and LF and space
                if (readCharacter == ' ' || readCharacter == 0x0A || readCharacter == 0x0D)
                {
                    if (temporaryRotationOrUVScale.empty())
                        continue;

                    temporaryPlane->setScaleV(parseFloat(temporaryRotationOrUVScale));
                    temporaryRotationOrUVScale.clear();

                    // Plane is done, we can push it now
                    temporaryBrush->pushPlane(temporaryPlane);

                    m_state = ParserState::InBrushExpectingPlaneOrEnd;
                    continue;
                }

                temporaryRotationOrUVScale.push_back(readCharacter);
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

    void MapParser::createPolygonsFromBrushes()
    {
        for (auto& entity : m_entities)
        {
            entity->createPolygonsFromBrushes();
        }
    }

    void MapParser::generateListOfAllUsedTextures()
    {
        for (auto& entity : m_entities)
        {
            auto texturesUsed = entity->generateListOfAllUsedTextures();
            for (auto& texture : texturesUsed)
            {
                auto iterator = std::find_if(m_usedTextures.begin(), m_usedTextures.end(), [&texture](const TextureInfo& x) { return texture == x.name; });
                if (iterator == m_usedTextures.end())
                {
                    TextureInfo info;
                    info.name = texture;
                    m_usedTextures.push_back(std::move(info));
                }
            }
        }
    }

    void MapParser::acquireDimensionsForUsedTextures()
    {
        // "../materials/IMPORTED/bsp" is the currently expected path for texturs
        auto basePath = std::filesystem::current_path().parent_path() /= "materials/IMPORTED/bsp";

        for (auto& texture : m_usedTextures)
        {
            auto path = (basePath / texture.name);
            path.replace_extension(".png");

            // Skip if not found
            if (!std::filesystem::exists(path))
            {
                DC_CORE_TRACE("  {0} not found, setting up as ERROR", texture.name + ".png");
                continue;
            }

            auto data = stbi_load((path).u8string().c_str(), &texture.width, &texture.height, nullptr, 0);
            stbi_image_free(data);

            // Some sanity checks
            if (texture.width > 65536 || texture.width < 0)
            {
                DC_CORE_WARN("{1}, Malformed texture width: {0}", texture.width, texture.name + ".png");
                continue;
            }
            if (texture.height > 65536 || texture.height < 0)
            {
                DC_CORE_WARN("{1}, Malformed texture height: {0}", texture.height, texture.name + ".png");
                continue;
            }

            // Finally, if everything is good, just set it as all good
            texture.missing = false;
        }

        DC_CORE_TRACE("List of all used textures: ");
        for (auto& texture : m_usedTextures)
        {
            DC_CORE_TRACE("  {0} => {1}x{2}", texture.name, texture.width, texture.height);
        }
    }

    void MapParser::calculateTextureCoordinates()
    {
        for (auto& entity : m_entities)
        {
            entity->calculateTextureCoordinates(m_usedTextures);
        }
    }

    void MapParser::sortVertices()
    {
        for (auto& entity : m_entities)
        {
            entity->sortVertices();
        }
    }

    float MapParser::parseFloat(const std::string& string)
    {
        // TODO: This probably needs some safety checks
        return std::stof(string);
    }

    bool MapParser::isValidTextureNameCharacter(char character)
    {
        // Uppercase letters
        if (character >= 65 && character <= 90)
            return true;

         // Lowercase letters
        if (character >= 97 && character <= 122)
            return true;

        // Numbers
        if (character >= 48 && character <= 57)
            return true;

        switch (character)
        {
        case '-': // Random textures
        case '+': // Triggerable textures
        case '~':
        case '_':
        case '!': // Water and liquids
        case '{': // Solid textures
        case '#':
            return true;

        default:
            return false;
        }
    }

    void MapParser::setAppropriatePlanePoint(Ref<Plane> plane, const PlanePoint& point)
    {
        switch (m_pointState)
        {
        case ParserPointState::DoingPoint1:
        {
            plane->setPoint1(point);
            return;
        }
        case ParserPointState::DoingPoint2:
        {
            plane->setPoint2(point);
            return;
        }
        case ParserPointState::DoingPoint3:
        {
            plane->setPoint3(point);
            return;
        }
        default:
            VERIFY_NOT_REACHED();
            return;
        }
    }

    void MapParser::incrementPlanePointState()
    {
        switch (m_pointState)
        {
        case ParserPointState::DoingPoint1:
            m_pointState = ParserPointState::ExpectingPoint2;
            break;
        case ParserPointState::ExpectingPoint2:
            m_pointState = ParserPointState::DoingPoint2;
            break;
        case ParserPointState::DoingPoint2:
            m_pointState = ParserPointState::ExpectingPoint3;
            break;
        case ParserPointState::ExpectingPoint3:
            m_pointState = ParserPointState::DoingPoint3;
            break;
        case ParserPointState::DoingPoint3:
            // Exit out of the plane point definitions
            m_pointState = ParserPointState::Outside;
            m_state = ParserState::InBrushExpectingPlaneTexture;
            break;
        default:
            VERIFY_NOT_REACHED();
            break;
        }
    }

    void MapParser::incrementUVState()
    {
        switch (m_parserUVstate)
        {
        case ParserUVState::DoingNormalX:
            m_parserUVstate = ParserUVState::DoingNormalY;
            break;
        case ParserUVState::DoingNormalY:
            m_parserUVstate = ParserUVState::DoingNormalZ;
            break;
        case ParserUVState::DoingNormalZ:
            m_parserUVstate = ParserUVState::DoingShift;
            break;
        case ParserUVState::DoingShift:
            m_parserUVstate = ParserUVState::Outside;
            break;
        default:
            VERIFY_NOT_REACHED();
            break;
        }
    }
}
