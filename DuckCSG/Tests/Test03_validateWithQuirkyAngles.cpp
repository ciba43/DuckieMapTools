#include "duckpch.h"
#include "Tests.h"

#include <glm/vec3.hpp>

#include "Brush.h"
#include "Entity.h"
#include "MapParser.h"
#include "Polygon.h"

namespace MapTools {

    TestResult Test03ValidateWithQuirkyAngles::run()
    {
        // Create a map parser
        MapParser parser("Tests/test03_invalid_intersection.map");

        auto result = parser.parse();

        // Fail if parser fails
        if (result != ParseReturnState::OK)
        {
            setFailDescription("Internal parser failure");
            return TestResult::Fail;
        }

        auto worldspawn = parser.entity(0);

        // This map consists of a single brush
        auto brush = worldspawn->brush(0);

        // Create polygons from planes
        brush->createPolygons();

        // Legal vertices when looking from the top
        glm::vec3 legalVerts[] = {
            // The bottom plane has these vertices:
            { 96, 64, 0 },      { 192, 64, 0 },
            { 144, -32, 0 },     { 192, -32, 0 },
            // While the top has these:
            { 192, 64, 96 },
            { 192, -32, 48 }
        };

        // Indices for bottom -         0, 1, 2, 3
        // Indices for the north side - 0, 1, 4
        // Indices for the east side -  1, 3, 4, 5
        // Indices for the south side - 2, 3, 5
        // Indices for the west side -  0, 2, 4, 5

        bool foundBottom, foundNorth, foundEast, foundSouth, foundWest;
        foundBottom = foundNorth = foundEast = foundSouth = foundWest = false;

#define HAS_VERT(index) brush->polygon(polID)->hasVertex(legalVerts[index])
#define HAS_THREE() (brush->polygon(polID)->vertexCount() == 3)
#define HAS_FOUR() (brush->polygon(polID)->vertexCount() == 4)

        for (size_t polID = 0; polID < 5; polID++)
        {
            // Vert 0 is valid for bottom, north and west
            if (HAS_VERT(0))
            {
                // Vert 1 is then valid for bottom and north
                if (HAS_VERT(1))
                {
                    // Vert 2 is then only valid for bottom
                    if (HAS_VERT(2))
                    {
                        if (HAS_VERT(3))
                        {
                            if (!HAS_FOUR())
                            {
                                setFailDescription("More than 4 vertices, while we did get valid 0, 1, 2, 3");
                                return TestResult::Fail;
                            }
                            // All is good, we've found bottom
                            if (!foundBottom)
                            {
                                foundBottom = true;
                                continue;
                            }
                            else
                            {
                                setFailDescription("Found the bottom polygon twice!");
                                return TestResult::Fail;
                            }
                        }
                        setFailDescription("Unexpected vertex coordinate with 0, 1, 2");
                        return TestResult::Fail;
                    }
                    // Vert 4 is then only valid for north
                    if (HAS_VERT(4))
                    {
                        if (!HAS_THREE())
                        {
                            setFailDescription("More than 3 vertices, while we did get valid 0, 1, 4");
                            return TestResult::Fail;
                        }
                        // All is good, we've found north
                        if (!foundNorth)
                        {
                            foundNorth = true;
                            continue;
                        }
                        else
                        {
                            setFailDescription("Found the north polygon twice!");
                            return TestResult::Fail;
                        }
                    }
                    setFailDescription("Unexpected vertex coordinate with 0, 1");
                    return TestResult::Fail;
                }
                // Vert 2 is only valid for west
                if (HAS_VERT(2))
                {
                    if (HAS_VERT(4))
                    {
                        if (HAS_VERT(5))
                        {
                            if (!HAS_FOUR())
                            {
                                setFailDescription("More than 4 vertices, while we did get valid 0, 2, 4, 5");
                                return TestResult::Fail;
                            }
                            // All is good, we've found west
                            if (!foundWest)
                            {
                                foundWest = true;
                                continue;
                            }
                            else
                            {
                                setFailDescription("Found the west polygon twice!");
                                return TestResult::Fail;
                            }
                        }
                        setFailDescription("Unexpected vertex coordinate with 0, 2, 4");
                        return TestResult::Fail;
                    }
                    setFailDescription("Unexpected vertex coordinate with 0, 2");
                    return TestResult::Fail;
                }
                setFailDescription("Unexpected vertex coordinate with 0");
                return TestResult::Fail;
            }
            // Vert 1 is only valid for east
            if (HAS_VERT(1))
            {
                if (HAS_VERT(3))
                {
                    if (HAS_VERT(4))
                    {
                        if (HAS_VERT(5))
                        {
                            if (!HAS_FOUR())
                            {
                                setFailDescription("More than 4 vertices, while we did get valid 1, 3, 4, 5");
                                return TestResult::Fail;
                            }
                            // All is good, we've found east
                            if (!foundEast)
                            {
                                foundEast = true;
                                continue;
                            }
                            else
                            {
                                setFailDescription("Found the west polygon twice!");
                                return TestResult::Fail;
                            }
                        }
                        setFailDescription("Unexpected vertex coordinate with 1, 3, 4");
                        return TestResult::Fail;
                    }
                    setFailDescription("Unexpected vertex coordinate with 1, 3");
                    return TestResult::Fail;
                }
                setFailDescription("Unexpected vertex coordinate with 1");
                return TestResult::Fail;
            }
            // Vert 2 is only for south
            if (HAS_VERT(2))
            {
                if (HAS_VERT(3))
                {
                    if (HAS_VERT(5))
                    {
                        if (!HAS_THREE())
                        {
                            setFailDescription("More than 3 vertices, while we did get valid 2, 3, 5");
                            return TestResult::Fail;
                        }
                        // All is good, we've found south
                        if (!foundSouth)
                        {
                            foundSouth = true;
                            continue;
                        }
                        else
                        {
                            setFailDescription("Found the south polygon twice!");
                            return TestResult::Fail;
                        }
                    }
                    setFailDescription("Unexpected vertex coordinate with 2, 3");
                    return TestResult::Fail;
                }
                setFailDescription("Unexpected vertex coordinate with 2");
                return TestResult::Fail;
            }
            setFailDescription("Unkown vertex coordinate found, bailing");
            return TestResult::Fail;
        }

        // Finally do a check
        if (!foundNorth || !foundWest || !foundEast || !foundSouth || !foundBottom)
        {
            setFailDescription("One of the sides wasn't found");
            return TestResult::Fail;
        }

        return TestResult::Pass;
    }
}
