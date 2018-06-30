#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include <iostream>

struct VulkanDebugger {
	VulkanDebugger();
public:

	//Creates the callbacks for the vulkan errors / warnings.
	void CreateValidationLayer(VkInstance& VulkanInstance);

	//Checks to make sure the GPU can be validated.
	bool checkValidationLayerSupport();

	//Returns size of the validation layers we say to check.
	inline uint32_t ValidationSize() { return validationLayers.size(); }
	
	//Returns a pointer const to the array of names;
	inline const char* const* ValidationNames() { return &validationLayers[0]; }

	//Prints out the list of avaliable Vulkan api extensions.
	void LogAvaliableVulkanExtensions();

	//Cleanup all of the Vulkan based instances.
	void Cleanup(VkInstance& instance);
protected:
	VkDebugReportCallbackEXT callback;
	VkDebugReportCallbackCreateInfoEXT CallbackCreationInfo = {};

	std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
};