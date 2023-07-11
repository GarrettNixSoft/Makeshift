project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Makeshift/vendor/spdlog/include",
		"%{wks.location}/Makeshift/src",
		"%{wks.location}/Makeshift/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links {
		"Makeshift"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "MK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MK_DIST"
		runtime "Release"
		optimize "on"