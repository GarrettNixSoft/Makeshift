project "Makeshift-Workshop"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Makeshift/vendor/spdlog/include",
		"%{wks.location}/Makeshift/src",
		"%{wks.location}/Makeshift/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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

	filter "files:assets/shaders/*"
		buildcommands '"C:/VulkanSDK/1.3.246.1/Bin/glslc.exe" -V -o "%{file.directory}/bin/%{file.name}.spv" "%{file.relpath}"'
		buildoutputs "%{file.directory}/bin/%{file.name}.spv"