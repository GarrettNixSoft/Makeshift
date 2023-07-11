include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Makeshift"
	architecture "x86_64"
	startproject "Makeshift-Workshop"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	
	solution_items {
		".editorconfig"
	}
	
	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Makeshift/vendor/GLFW"
	include "Makeshift/vendor/Glad"
	include "Makeshift/vendor/imgui"
	include "Makeshift/vendor/vma"
	include "Makeshift/vendor/yaml-cpp"

group ""

include "Makeshift"
include "Sandbox"
include "Makeshift-Workshop"