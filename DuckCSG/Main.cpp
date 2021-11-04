#include "duckpch.h"

#include "Log.h"

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
    MapTools::Test01ParseOneBrush test01;
    auto result = test01.run();

    if (result == MapTools::TestResult::Pass)
        DC_CORE_INFO("01: PASS");
    else
        DC_CORE_ERROR("01: FAIL : {0}", test01.failDescription());
#endif

    // Parse a test file
    MapTools::MapParser mapParser("tests/test01_fail.map");

    auto parserResult = mapParser.parse();
    if (parserResult == MapTools::ParseReturnState::OK)
        DC_CORE_INFO("All good :)");
    else
        DC_CORE_ERROR("Yikes, something went wrong");

    // Create Polygons from Brushes
    mapParser.createPolygonsFromBrushes();



    return 0;
}
