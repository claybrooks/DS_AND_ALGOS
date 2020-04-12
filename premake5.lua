workspace "DS_AND_ALGO"
	architecture "x86_64"
	startproject "Test"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["src"] = "src"

--group "Dependencies"
--	include "Hazel/vendor/GLFW"
--	include "Hazel/vendor/Glad"
--	include "Hazel/vendor/imgui"

--group ""

project "Datastructures"
	location "Datastructures"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/heaps/**.hpp",
		"%{prj.name}/heaps/**.cpp",
		"%{prj.name}/GenLib.cpp"
	}

	defines
	{
	}

	includedirs
	{
	}

	links 
	{
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"

project "Algorithms"
	location "Algorithms"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/sort/**.hpp",
		"%{prj.name}/sort/**.cpp",
		"%{prj.name}/search/**.hpp",
		"%{prj.name}/search/**.cpp",
		"%{prj.name}/GenLib.cpp"
	}

	defines
	{
	}

	includedirs
	{
		"Datastructures/heaps",
	}

	links 
	{
		"Datastructures"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"

project "Test"
	location "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/main.cpp",
		"%{prj.name}/HeapDriver.in"
	}

	includedirs
	{
		"Datastructures/heaps",
		"Algorithms/sort",
		"Algorithms/search"
	}

	links
	{
		"Datastructures",
		"Algorithms"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
