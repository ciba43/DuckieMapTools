#include "duckpch.h"
#include "Tests.h"

#include "Brush.h"
#include "Entity.h"
#include "MapParser.h"
#include "Plane.h"

namespace MapTools {

    TestResult Test01ParseOneBrush::run()
    {
        // Create a map parser
        MapParser parser("Tests/test01_fail.map");

        auto result = parser.parse();

        // Fail if parser fails
        if (result != ParseReturnState::OK)
        {
            setFailDescription("Internal parser failure");
            return TestResult::Fail;
        }

        // Only 1 entity in the test file
        if (parser.entityCount() != 1)
        {
            setFailDescription("Bad parsed entity count (expected 1, got " + std::to_string(parser.entityCount()) + ")");
            return TestResult::Fail;
        }

        auto worldspawn = parser.entity(0);

        if (worldspawn == nullptr)
        {
            setFailDescription("Entity 0 is invalid");
            return TestResult::Fail;
        }

        // Classname must exist and be worldspawn
        auto classname = worldspawn->findPropertySlow("classname");

        if (classname != "worldspawn")
        {
            setFailDescription("Entity 0 isn't worldspawn");
            return TestResult::Fail;
        }

        // Worldspawn has 9 properties
        if (worldspawn->propertyCount() != 9)
        {
            setFailDescription("Bad parsed property count (expected 9, got " + std::to_string(worldspawn->propertyCount()) + ")");
            return TestResult::Fail;
        }

        // Only 1 brush
        if (worldspawn->brushCount() != 1)
        {
            setFailDescription("Bad parsed brush count (expected 1, got " + std::to_string(worldspawn->brushCount()) + ")");
            return TestResult::Fail;
        }

        auto brush = worldspawn->brush(0);

        // The brush should contain 6 planes
        if (brush->planeCount() != 6)
        {
            setFailDescription("Bad parsed plane count (expected 1, got " + std::to_string(brush->planeCount()) + ")");
            return TestResult::Fail;
        }

        auto plane = brush->plane(0);

        // First point
        if (plane->point1().x != 64 || plane->point1().y != 96 || plane->point1().z != 128)
        {
            setFailDescription("Plane point isn't correct");
            return TestResult::Fail;
        }

        if (plane->texture() != "Basewall")
        {
            setFailDescription("Plane texture isn't correct (expected Basewall, got " + plane->texture() + ")");
            return TestResult::Fail;
        }

        return TestResult::Pass;
    }
}