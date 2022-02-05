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

    // Step 1: Parse a test file
    MapTools::MapParser mapParser("tests/test03_invalid_intersection.map");

    auto parserResult = mapParser.parse();
    switch (parserResult)
    {
    case MapTools::ParseReturnState::OK:
        break;
    case MapTools::ParseReturnState::MissingFile:
        DC_CORE_ERROR("Error: Missing input file {0}", mapParser.mapName());
        return 1;
    default:
        DC_CORE_CRITICAL("Fatal error during compilation, exiting");
        return 1;
    }

    // Step 2: Create Polygons from Brushes
    mapParser.createPolygonsFromBrushes();
    
    // Step 3: Generate a list of all necessary textures
    
    // Step 4: Attempt to acquire the dimensions of the used texture

    // Step 5: Calculate texture coordinates for each point

    return 0;
}
