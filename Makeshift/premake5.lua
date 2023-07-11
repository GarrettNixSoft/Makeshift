project "Makeshift"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mkpch.hpp"
	pchsource "src/mkpch.cpp"

	files {
		"src/**.hpp",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		
		"vendor/imguizmo/ImGuizmo.h",
		"vendor/imguizmo/ImGuizmo.cpp"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",

		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.vma}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",

		"%{Library.Vulkan}"
	}

	filter "files:vendor/imguizmo/**.cpp"
	flags { "NoPCH" } -- This won't work for whatever reason, must manually set this flag each time the project is regenerated

	filter "system:windows"
		systemversion "latest"

		defines {
		}
		
	filter "configurations:Debug"
		defines "MK_DEBUG"
		runtime "Debug"
		symbols "on"

		links {
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "MK_RELEASE"
		runtime "Release"
		optimize "on"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "MK_DIST"
		runtime "Release"
		optimize "on"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}