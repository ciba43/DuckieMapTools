#include "duckpch.h"

#include "MapParser.h"

#ifdef RUN_TESTS_ON_STARTUP
#include "Tests/Tests.h"
#endif
int main(int argc, char** argv)
{
    DuckCSG::Log::init();

    DC_CORE_INFO("DuckCSG map compiler for Duckie game engine");
    DC_CORE_TRACE("Argument parsing isn't implemented, working on a test map ...");

#ifdef RUN_TESTS_ON_STARTUP
    // Run tests on startup
    DC_CORE_TRACE("Running tests ...");

    RUN_TEST(Test01ParseOneBrush);
    RUN_TEST(Test02ValidateSimplePolygons);
    RUN_TEST(Test03ValidateWithQuirkyAngles);
#endif

    // Step 1: Parse a test file
    DC_CORE_INFO("[step1]: Creating a parser object ...");
    MapTools::MapParser mapParser("tests/test03_invalid_intersection.map");

    DC_CORE_INFO("[step1]: Parsing map ...");
    auto parserResult = mapParser.parse();
    switch (parserResult)
    {
    case MapTools::ParseReturnState::OK:
        DC_CORE_TRACE("Map parsed successfully");
        break;
    case MapTools::ParseReturnState::MissingFile:
        DC_CORE_ERROR("Error: Missing input file {0}", mapParser.mapName());
        return 1;
    default:
        DC_CORE_CRITICAL("Fatal error during compilation, exiting");
        return 1;
    }

    // Step 2: Create Polygons from Brushes
    DC_CORE_INFO("[step2]: Creating polygons from brushes ...");
    mapParser.createPolygonsFromBrushes();

    // Step 3: Generate a list of all necessary textures
    DC_CORE_INFO("[step3]: Generating a list of all used textures ...");
    mapParser.generateListOfAllUsedTextures();

    // Step 4: Attempt to acquire dimensions for all used textures
    DC_CORE_INFO("[step4]: Attempting to acquire dimensions for all used textures ...");
    mapParser.acquireDimensionsForUsedTextures();

    // Step 5: Calculate texture coordinates for each point
    DC_CORE_INFO("[step5]: Calculating texture coordinates for each point ...");
    mapParser.calculateTextureCoordinates();

    // Step 6: Perform texture coordinate normalization
    DC_CORE_INFO("[step6]: Performing texture coordinate normalization ...");
    DC_CORE_WARN("THIS STEP ISN'T DONE");

    // Step 7: Sort vertices
    DC_CORE_INFO("[step7]: Sorting vertices ...");
    mapParser.sortVertices();

    return 0;
}
