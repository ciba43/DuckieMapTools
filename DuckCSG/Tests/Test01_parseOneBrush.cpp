#include "duckpch.h"
#include "Tests.h"

#include "Entity.h"
#include "MapParser.h"

namespace MapTools {

    TestResult Test01ParseOneBrush::run()
    {
        // Create a map parsers
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

        auto classname = worldspawn->findPropertySlow("classname");

        if (classname != "worldspawn")
        {
            setFailDescription("Entity 0");
            return TestResult::Fail;
        }

        // Test map's worldspawn has 9 properties
        if (worldspawn->propertyCount() != 9)
        {
            setFailDescription("Bad parsed property count (expected 9, got " + std::to_string(worldspawn->propertyCount()) + ")");
            return TestResult::Fail;
        }

        return TestResult::Pass;
    }
}