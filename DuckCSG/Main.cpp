#include "duckpch.h"

#include "Log.h"

#include "MapParser.h"

int main(int argc, char** argv)
{
    DuckCSG::Log::init();

    DC_CORE_INFO("DuckCSG map compiler for Duckie game engine");

    // Parse a map
    MapTools::MapParser mapParser("test01_fail.map");

    auto parserResult = mapParser.parse();
    if (parserResult == MapTools::ParseReturnState::OK)
    {
        DC_CORE_INFO("All good :)");
    }
    else
    {
        DC_CORE_ERROR("Yikes, something went wrong");
    }

    return 0;
}
