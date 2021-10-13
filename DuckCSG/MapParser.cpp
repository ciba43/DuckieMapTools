#include "duckpch.h"
#include "MapParser.h"

namespace MapTools {

    MapParser::MapParser(std::string map)
    {
        m_map = map;
    }

    ParseReturnState MapParser::parse()
    {
        // Log if we've already parsed the file
        if (m_parsed)
            DC_CORE_INFO("[Parser] Already parsed but reparsing requested");
        m_parsed = true;

        // Load file
        std::ifstream mapFile;
        mapFile.open(m_map);

        if (!mapFile.is_open())
            return ParseReturnState::MissingFile;

        std::string line;
        while (std::getline(mapFile, line))
        {
            DC_CORE_TRACE(line);
        }

        mapFile.close();

        return ParseReturnState::OK;
    }
}
