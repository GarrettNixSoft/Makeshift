#pragma once

#include "Makeshift/Renderer/GraphicsContext.hpp"

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

#include <string>

struct GLFWwindow;

namespace Makeshift {

	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;

		inline static VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
		inline static VkDevice& GetDevice() { return device; }

		// TODO: determine what other objects need to be exposed, if any

		void cleanup();
	private:
		GLFWwindow* windowHandle;

		static VkInstance instance;
		static VkDebugUtilsMessengerEXT debugMessenger;

		static VkPhysicalDevice physicalDevice;
		static VkDevice device;
		static VkQueue graphicsQueue;

		static VkSurfaceKHR surface;
		static VkQueue presentQueue;

		static VkSwapchainKHR swapChain;
		static std::vector<VkImage> swapChainImages;
		static VkFormat swapChainImageFormat;
		static VkExtent2D swapChainExtent;
		static std::vector<VkImageView> swapChainImageViews;

		static VkPipeline graphicsPipeline;

		static std::unordered_map<std::string, VkRenderPass> s_RenderPasses;
		static std::unordered_map<std::string, VkPipeline> s_Pipelines;
	};

}
