workspace "Makeshift"
	architecture "x64"

	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solution (root) dir
IncludeDir = {}
IncludeDir["GLFW"] = "Makeshift/vendor/GLFW/include"
IncludeDir["Glad"] = "Makeshift/vendor/Glad/include"
IncludeDir["ImGui"] = "Makeshift/vendor/imgui"

group "Dependencies"
	include "Makeshift/vendor/GLFW"
	include "Makeshift/vendor/Glad"
	include "Makeshift/vendor/imgui"

group ""

project "Makeshift"
	location "Makeshift"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mkpch.hpp"
	pchsource "Makeshift/src/mkpch.cpp"

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		runtime "Debug"
		systemversion "latest"

		defines {
			"MK_PLATFORM_WINDOWS",
			"MK_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "MK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MK_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Makeshift/vendor/spdlog/include",
		"Makeshift/src"
	}

	links {
		"Makeshift"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"MK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MK_DIST"
		runtime "Release"
		optimize "On"
