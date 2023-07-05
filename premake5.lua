workspace "Makeshift"
	architecture "x64"

	startproject "Makeshift-Workshop"

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
IncludeDir["glm"] = "Makeshift/vendor/glm"
IncludeDir["Vulkan"] = "C:/VulkanSDK/1.3.246.1/Include"
IncludeDir["vma"] = "Makeshift/vendor/vma/include"
IncludeDir["stb_image"] = "Makeshift/vendor/stb_image"
IncludeDir["entt"] = "Makeshift/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Makeshift/vendor/yaml-cpp/include"

group "Dependencies"
	include "Makeshift/vendor/GLFW"
	include "Makeshift/vendor/Glad"
	include "Makeshift/vendor/imgui"
	include "Makeshift/vendor/vma"
	include "Makeshift/vendor/yaml-cpp"

group ""

project "Makeshift"
	location "Makeshift"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mkpch.hpp"
	pchsource "Makeshift/src/mkpch.cpp"

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.vma}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"C:/VulkanSDK/1.3.246.1/Lib/vulkan-1.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"GLFW_INCLUDE_NONE"
		}
		
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Makeshift/vendor/spdlog/include",
		"Makeshift/src",
		"Makeshift/vendor",
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

project "Makeshift-Workshop"
	location "Makeshift-Workshop"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Makeshift/vendor/spdlog/include",
		"Makeshift/src",
		"Makeshift/vendor",
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

	filter "files:assets/shaders/*"
		buildcommands '"C:/VulkanSDK/1.3.246.1/Bin/glslc.exe" -V -o "%{file.directory}/bin/%{file.name}.spv" "%{file.relpath}"'
		buildoutputs "%{file.directory}/bin/%{file.name}.spv"