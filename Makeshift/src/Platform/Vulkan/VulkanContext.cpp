#include "mkpch.hpp"
#include "VulkanContext.hpp"
#include "VulkanInit.hpp"

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

namespace Makeshift {

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	VulkanContext::VulkanContext(GLFWwindow* window) : windowHandle{ window } {
		MK_CORE_ASSERT(windowHandle, "windowHandle is null!");
	}

	void VulkanContext::init() {
		instance = VulkanInit::createInstance();
		debugMessenger = VulkanInit::createDebugMessenger(instance);
		surface = VulkanInit::createSurface(instance, windowHandle);
		physicalDevice = VulkanInit::pickPhysicalDevice(instance, surface);
		device = VulkanInit::createLogicalDevice(physicalDevice, surface, graphicsQueue, presentQueue);
		swapChain = VulkanInit::createSwapChain(surface, physicalDevice, device, windowHandle, swapChainImages, swapChainImageFormat, swapChainExtent);
	}

	void VulkanContext::swapBuffers() {

	}

	void VulkanContext::cleanup() {
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(device, swapChain, nullptr);

		vkDestroyDevice(device, nullptr);

		if (enableValidationLayers) {
			VulkanInit::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);
	}

}