#include "duckpch.h"

#include "Log.h"

int main(int argc, char** argv)
{
    DuckCSG::Log::init();

    DC_CORE_INFO("DuckCSG");

    return 0;
}
