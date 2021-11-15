#include "duckpch.h"

#include "MapParser.h"

#ifdef RUN_TESTS_ON_STARTUP
#include "Tests/Tests.h"
#endif
int main(int argc, char** argv)
{
    DuckCSG::Log::init();

    DC_CORE_INFO("DuckCSG map compiler for Duckie game engine");

#ifdef RUN_TESTS_ON_STARTUP
    // Run tests on startup
    DC_CORE_TRACE("Running tests ...");

    RUN_TEST(Test01ParseOneBrush);
    RUN_TEST(Test02ValidateSimplePolygons);
    RUN_TEST(Test03ValidateWithQuirkyAngles);
#endif

    // Parse a test file
    MapTools::MapParser mapParser("tests/test03_invalid_intersection.map");

    auto parserResult = mapParser.parse();
    if (parserResult == MapTools::ParseReturnState::OK)
        DC_CORE_INFO("All good :)");
    else
        DC_CORE_ERROR("Yikes, something went wrong");

    // Create Polygons from Brushes
    mapParser.createPolygonsFromBrushes();



    return 0;
}
