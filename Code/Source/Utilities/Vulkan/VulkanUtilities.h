#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct QueueFamilyIndex {
	int QueueID = -1;
	int PresentQueueID = -1;

	bool isComplete() {
		return QueueID >= 0 && PresentQueueID >= 0;
	}
};

class VulkanUtilities
{
public:
	static VulkanUtilities* instance;

	static VulkanUtilities *GetInstance()
	{
		if (!instance)
			instance = new VulkanUtilities;
		return instance;
	}

	VulkanUtilities();
	~VulkanUtilities();

	//Do this after setting every window property in the GLOBALS.
	void Initilize(GLFWwindow& GLFWWindow);
	
	//Creates the instace for vulkan, but also sets the global instance value.
	void CreateVulkanInstance();

	//Gets the glfwExtenstions required for Vulkan.
	std::vector<const char*> getGLFWExtensions();

	//Selects the graphics card to use for Vulkan.
	void PickGraphicsDevice();

	//Finds the best card to use for Vulkan.
	VkPhysicalDevice FindMostSuitableCard(const std::vector<VkPhysicalDevice>& AvailableCards);

	//Do the scoring method of a Vulkan gpu to figure out the best one out of them all.
	int GetDeviceScore(VkPhysicalDevice& Device);

	//Returns the device name...
	const char* GetDeviceName(VkPhysicalDevice& Device);

	//Finds the GPU queue allowances to use the Vulkan api with.
	QueueFamilyIndex GetQueueFamily();

	//Checks to make sure that the PhysicalDevice has ATLEAST a graphics queue.
	bool isBindedDeviceSuitable();

	//Creates the logical device to bridge between Vulkan/C++ & the graphical device.
	void CreateLogicalDevice();

	//Creates the surface between glfw and Vulkan.
	void CreateSurface();

	//Checks to make sure you can use the swapchain.
	bool CheckBindedDeviceExtensionSupport();

public:
	bool EnableValidation;
	VkApplicationInfo AppInfo = {};
	VkDeviceCreateInfo DeviceCreateInfo = {};
	VkInstanceCreateInfo InstanceCreationInfo = {};
	VkDeviceQueueCreateInfo QueueCreateInfo = {};
	VkPhysicalDeviceFeatures PhysicalDeviceFeatures = {};
	VkInstance VulkanInstance = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkDevice VulkanDevice = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkSurfaceKHR VulkanSurface = {};
	class VulkanDebugger* VulkanDebug;

	GLFWwindow* GameWindow;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};


