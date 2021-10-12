-- Duckie Map tools premake5 script

workspace "DuckieMapTools"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories
includeDir = {}
includeDir["spdlog"] = "subprojects/spdlog/include"

-- spdlog project premake
project "spdlog"
    location "subprojects/spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("temp/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/**.h"
    }

    includedirs
    {
        "%{includeDir.spdlog}"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- DuckCSG project
project "DuckCSG"
    location "DuckCSG"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("temp/" .. outputdir .. "/%{prj.name}")

    pchheader "duckpch.h"
    pchsource "DuckCSG/duckpch.cpp"

    files
    {
        "%{prj.location}/**.h",
        "%{prj.location}/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "PREMAKE",
    }

    includedirs
    {
        "%{prj.location}",
        "%{includeDir.spdlog}"
    }

    filter "system:linux"
        links
        {
            "pthread",
            "fmt"
        }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines
        {
            "DUCKIEMAP_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DUCKIEMAP_RELEASE"
        runtime "Release"
        optimize "on"
