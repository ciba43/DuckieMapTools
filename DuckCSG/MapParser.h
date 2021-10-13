#pragma once

#include <string>

namespace MapTools {

    enum class ParseReturnState
    {
        OK,
        MissingFile
    };

    class MapParser
    {
    public:
        MapParser(std::string map);

        // Do the actual parsing
        ParseReturnState parse();

        // Get all of the parsed properties

    private:
        std::string m_map;
        bool m_parsed = false;
    };
}
