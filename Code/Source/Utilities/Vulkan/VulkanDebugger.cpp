#include "VulkanDebugger.h"
#include "../Log.h"
#include <vector>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) {

	VulkanDebugLog("validation layer: %s", msg)

	//std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


VulkanDebugger::VulkanDebugger()
{
}

bool VulkanDebugger::checkValidationLayerSupport() {
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

void VulkanDebugger::LogAvaliableVulkanExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	Log("Available Vulkan extensions:");

	for (const auto& extension : extensions)
	{
		LogList(extension.extensionName);
	}
}

void VulkanDebugger::CreateValidationLayer(VkInstance& VulkanInstance)
{
	if (!checkValidationLayerSupport() && glfwVulkanSupported()) {
		VulkanFatalLog("Validation instance couldn't not be found!")
	}

	CallbackCreationInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	CallbackCreationInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	CallbackCreationInfo.pfnCallback = debugCallback;

	if (CreateDebugReportCallbackEXT(VulkanInstance, &CallbackCreationInfo, nullptr, &callback) != VK_SUCCESS) {
		VulkanFatalLog("Could not create a Vulkan debug context!");
	}
	else
	{
		VulkanLog("Successfully created a Vulkan debug context!");
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void VulkanDebugger::Cleanup(VkInstance& instance)
{
	DestroyDebugReportCallbackEXT(instance, callback, nullptr);
}
