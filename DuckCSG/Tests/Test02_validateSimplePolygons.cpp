#include "duckpch.h"
#include "Tests.h"

#include "MapParser.h"

namespace MapTools {

    TestResult Test02ValidateSimplePolygons::run()
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

        return TestResult::Pass;
    }
}