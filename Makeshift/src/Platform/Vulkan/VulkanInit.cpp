#include "mkpch.hpp"
#include "VulkanInit.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Makeshift {

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	VkInstance VulkanInit::createInstance(const char* appName) {
		// Check that validation is available if necessary
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			MK_CORE_ASSERT(false, "Vulkan validation layers not available!");
		}

		// Describe the application
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Makeshift";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Define the instance creation config
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Add validation layers
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// TEST: Log extensions
		std::vector<VkExtensionProperties> availableExtensions = getAvailableExtensions();
		MK_CORE_TRACE("Available Extensions:");
		for (const auto& extension : availableExtensions) {
			MK_CORE_TRACE("\t{0}", extension.extensionName);
		}

		// Check for GLFW's required extensions
		std::vector<const char*> extensions = getRequiredExtensions();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Create and return the instance
		VkInstance result;

		if (vkCreateInstance(&createInfo, nullptr, &result) != VK_SUCCESS) {
			MK_CORE_ASSERT(false, "Failed to create Vulkan instance!");
		}

		return result;
	}

	std::vector<const char*> VulkanInit::getRequiredExtensions() {

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		// DEBUG: list GLFW extensions
		MK_CORE_TRACE("GLFW required extensions:");
		for (const auto& extension : extensions) {
			MK_CORE_TRACE("\t {0}", extension);
		}

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	std::vector<VkExtensionProperties> VulkanInit::getAvailableExtensions() {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		return extensions;
	}

	bool VulkanInit::checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	VkDebugUtilsMessengerEXT VulkanInit::createDebugMessenger(VkInstance instance) {
		if (!enableValidationLayers) return VK_NULL_HANDLE;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		VkDebugUtilsMessengerEXT result;

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &result) != VK_SUCCESS) {
			MK_CORE_ASSERT(false, "Vulkan debug messenger creation failed!");
		}

		return result;
	}

	VkPhysicalDevice VulkanInit::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
		// Count available devices
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		// If no Vulkan-supporting devices are found, abort
		if (deviceCount == 0) {
			MK_CORE_ASSERT(false, "No devices with Vulkan support found!");
		}

		// list all devices available
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices) {
			int score = rateDeviceSuitability(device, surface);
			candidates.insert(std::make_pair(score, device));
		}

		// Pick one of the devices
		VkPhysicalDevice result;

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0) {
			result = candidates.rbegin()->second;
		}
		else {
			MK_CORE_ASSERT(false, "Failed to find a suitable physical device!");
		}

		// Log device details
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(result, &deviceProperties);
		MK_CORE_INFO("SELECTED DEVICE:");
		MK_CORE_INFO("\tName: {0}", deviceProperties.deviceName);
		MK_CORE_INFO("\tVendor: {0}", deviceProperties.vendorID);
		MK_CORE_INFO("\tDriver: {0}", deviceProperties.driverVersion);
		MK_CORE_INFO("\tAPI ver: {0}", deviceProperties.apiVersion);

		return result;

	}

	int VulkanInit::rateDeviceSuitability(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		// get the device properties and features
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		// FEATURE BONUSES and WEIGHTS
		int score = 0;
		
		// discrete GPUs are heavily preferred
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// integrated GPUs are weighed less than discrete ones
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
			score += 100;
		}
		
		// Maximum possible size of textures affects graphics quality
		score += deviceProperties.limits.maxImageDimension2D;

		// DISQUALIFIERS

		// must have all required device extensions
		if (!checkDeviceExtensionSupport(physicalDevice)) {
			MK_CORE_TRACE("Cannot use {0}: Required device extension(s) not supported", deviceProperties.deviceName);
			return 0;
		}

		// must have adequate swap chain support
		bool swapChainAdequate = false;
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

		if (!swapChainAdequate) {
			MK_CORE_TRACE("Cannot use {0}: Inadequate swap chain support", deviceProperties.deviceName);
			return 0;
		}
		
		// we're not going to use a CPU
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
			MK_CORE_TRACE("Cannot use {0}: Device is a CPU", deviceProperties.deviceName);
			return 0;
		}

		// must have all required command queues
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

		if (!indices.isComplete()) {
			MK_CORE_TRACE("Cannot use {0}: Incomplete queue families", deviceProperties.deviceName);
			return 0;
		}

	}

	QueueFamilyIndices VulkanInit::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		// count the queue families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		// retrieve all the queue families
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		// search the queue families for our requirements
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			// find the graphics queue
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			// find the present queue
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			// we can break earlyy if we've found everything we need
			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	VkDevice VulkanInit::createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkQueue& graphicsQueue, VkQueue& presentQueue) {
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

		// create queues
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// specify required features when needed (none for now)
		VkPhysicalDeviceFeatures deviceFeatures{};

		// creating the logical device
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		// enable validation layers on the device
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		// create and return the device handle
		VkDevice result;

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &result) != VK_SUCCESS) {
			MK_CORE_ASSERT(false, "Failed to create logical device!");
		}

		// fetch the graphics queue for the caller
		vkGetDeviceQueue(result, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(result, indices.presentFamily.value(), 0, &presentQueue);

		return result;
	}

	VkSurfaceKHR VulkanInit::createSurface(VkInstance instance, GLFWwindow* window) {
		VkSurfaceKHR result;

		if (glfwCreateWindowSurface(instance, window, nullptr, &result) != VK_SUCCESS) {
			MK_CORE_ASSERT(false, "Failed to create window surface!");
		}

		return result;
	}

	bool VulkanInit::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VkSurfaceFormatKHR VulkanInit::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats) {

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];

	}

	VkPresentModeKHR VulkanInit::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {

		// prefer Mailbox (triple buffering) if available
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		// fallback to double buffering (VSync)
		return VK_PRESENT_MODE_FIFO_KHR;

	}

	VkExtent2D VulkanInit::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {

		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}

	}

	VkSwapchainKHR VulkanInit::createSwapChain(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device, GLFWwindow* window,
		std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, VkExtent2D& swapChainExtent) {
		// check what the device supports for this surface
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

		// choose optimal settings, store format and extent for caller
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities, window);

		// request one more than the minimum number of swap chain images, up to the maximum
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		// configure the swap chain
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = swapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		// configure queue families
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		// don't use any pre transforms
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

		// don't blend this window with other windows
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		// clipping helps reduce wasted performance when part of the window is occluded by another window, etc.
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		// no old swap chain (yet)
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		// create and return the swap chain
		VkSwapchainKHR result;

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &result) != VK_SUCCESS) {
			MK_CORE_ASSERT(false, "Failed to create swap chain!");
		}

		// fetch the swap chain images for the caller
		vkGetSwapchainImagesKHR(device, result, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, result, &imageCount, swapChainImages.data());

		return result;

	}

	void VulkanInit::createImageViews(VkDevice device, const std::vector<VkImage>& swapChainImages, VkFormat& swapChainImageFormat, std::vector<VkImageView>& swapChainImageViews) {
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
				MK_CORE_ASSERT(false, "Failed to create image views!");
			}
		}
	}

}