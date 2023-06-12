#pragma once

#include "Makeshift/Renderer/GraphicsContext.hpp"

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

struct GLFWwindow;

namespace Makeshift {

	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;

		void cleanup();
	private:
		GLFWwindow* windowHandle;

		VkInstance instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue graphicsQueue = VK_NULL_HANDLE;

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkQueue presentQueue = VK_NULL_HANDLE;

		VkSwapchainKHR swapChain = VK_NULL_HANDLE;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
	};

}