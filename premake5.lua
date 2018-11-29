workspace "ECS"
    architecture "x64"
    configurations { 
       "Debug", 
       "Release" 
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ECS"
    kind "SharedLib"
    location "ECS/"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    toolset ("clang")

    files { 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
    }

    filter { "action:gmake*", "toolset:gcc or clang" }
        buildoptions {
            "-Wall", 
            "-Wextra", 
            "-Werror", 
            "-Wpendantic"
        }

    filter "system:linux"
        cppdialect "C++14"
        staticruntime "On"
        systemversion "latest"

        defines {
            "ECS_LINUX"
        }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ECS_WINDOWS",
			"ECS_EXPORT_API"
		}


    filter "configurations:Debug"
        defines { 
            "ECS_DEBUG",
            "DEBUG"
        }
        symbols "On"

    filter "configurations:Release"
        defines {
            "ECS_RELEASE" 
        }
        optimize "On"

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
    }

project "Sandbox"
    kind "ConsoleApp"
    location "Sandbox/"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}/")
    toolset ("clang")

    files { 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
    }

    links {
        "ECS"
    }

    includedirs {
        "%{prj.name}/../ECS/"
    }

    filter { "action:gmake*", "toolset:gcc or clang" }
        buildoptions {
            "-Wall", 
            "-Wextra", 
            -- "-Werror"
        }

    filter "system:linux"
        cppdialect "C++14"
        staticruntime "On"
        systemversion "latest"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"