
-- Makeshift Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Makeshift/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Makeshift/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Makeshift/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Makeshift/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Makeshift/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Makeshift/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Makeshift/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Makeshift/vendor/imguizmo"

IncludeDir["shaderc"] = "%{wks.location}/Makeshift/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Makeshift/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["vma"] = "%{wks.location}/Makeshift/vendor/vma/include"


LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Makeshift/vendor/VulkanSDK/Lib"


Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"