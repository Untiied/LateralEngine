#include "VulkanUtilities.h"
#include "VulkanDebugger.h"
#include "../Globals.h"
#include "../Log.h"
#include <string>
#include <map>
#include <set>

VulkanUtilities *VulkanUtilities::instance = 0;

VulkanUtilities::VulkanUtilities()
{
#ifdef NDEBUG
	EnableValidation = false;
#else
	EnableValidation = true;
	VulkanDebug = new VulkanDebugger;
#endif
}

void VulkanUtilities::Initilize(GLFWwindow& GLFWWindow)
{
	GameWindow = &GLFWWindow;
	CreateVulkanInstance();
	if (EnableValidation)
		VulkanDebug->CreateValidationLayer(VulkanInstance);
	CreateSurface();
	PickGraphicsDevice();
	CreateLogicalDevice();
}

void VulkanUtilities::CreateVulkanInstance()
{
	//Sets the basic application information. Totally optional.
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = GlobalVariables::Application::ApplicationName;
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = GlobalVariables::Application::EngineName;
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	//Sets the intermediate steps for creation information.
	InstanceCreationInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceCreationInfo.pApplicationInfo = &AppInfo;
	//Gets the required amount of extensions for vulkan...
	//You should add a check to make sure it's not null.
	auto glfwExtensionInfo = getGLFWExtensions();
	InstanceCreationInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensionInfo.size());
	InstanceCreationInfo.ppEnabledExtensionNames = &glfwExtensionInfo[0];
		//If the validation layers are enabled, we have to tell Vulkan.
		if (EnableValidation)
		{
			InstanceCreationInfo.enabledLayerCount = static_cast<uint32_t>(VulkanDebug->ValidationSize());
			InstanceCreationInfo.ppEnabledLayerNames = VulkanDebug->ValidationNames();
		}
		else
		{
			InstanceCreationInfo.enabledLayerCount = 0;
		}
	//Uses the information above to create and instance of Vulkan.
	VkResult InstanceResult = vkCreateInstance(&InstanceCreationInfo, nullptr, &VulkanInstance);
		if(EnableValidation)
		{
			VulkanDebug->LogAvaliableVulkanExtensions();
		}

	if (InstanceResult != VK_SUCCESS) {
		VulkanFatalLog("Failed to create vulkan instance!");
	}
	else {
		VulkanLog("Successfully created a vulkan instance!");
	}
}

void VulkanUtilities::PickGraphicsDevice()
{
	//Just gathers the amount of graphical devices.
	uint32_t DeviceCount;
	VkResult FoundDevices = vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, nullptr);
	if (FoundDevices != VK_SUCCESS) {
		VulkanFatalLog("No GPU devices found!")
	}
	else {
		//Array to hold them all inorder for us to check their effectiveness.
		std::vector<VkPhysicalDevice> Devices(DeviceCount);

		vkEnumeratePhysicalDevices(VulkanInstance, &DeviceCount, Devices.data());
		VulkanLog("Found %d GPUS ready for Vulkan usage", DeviceCount);
		PhysicalDevice = FindMostSuitableCard(Devices);
		//Do I need to check here???
		if (isBindedDeviceSuitable()) {
			VulkanLog("GPU selected: %s", GetDeviceName(PhysicalDevice));
		}
		else {
			VulkanFatalLog("Your GPU can not even use Vulkan...")
		}

	}
}

void VulkanUtilities::CreateLogicalDevice()
{
	QueueFamilyIndex index = GetQueueFamily();
	float QueuePriorities = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { index.QueueID,  index.PresentQueueID };
	//Sets up the Queue/Presentation creation information.
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &QueuePriorities;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	//QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	//QueueCreateInfo.queueFamilyIndex = index.QueueID;
	//QueueCreateInfo.queueCount = 1;
	//QueueCreateInfo.pQueuePriorities = &QueuePriorities;

	//Next step for the creations info????
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	DeviceCreateInfo.pEnabledFeatures = &PhysicalDeviceFeatures;
	DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	DeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (EnableValidation) 
	{
		DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VulkanDebug->ValidationSize());
		DeviceCreateInfo.ppEnabledLayerNames = VulkanDebug->ValidationNames();
	}
	else
	{
		DeviceCreateInfo.enabledLayerCount = 0;
	}

	VkResult CreatedDevice = vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, nullptr, &VulkanDevice);
	if (CreatedDevice != VK_SUCCESS)
	{
		VulkanFatalLog("Could not create the Vulkan Logical Device!")
	}
	else
	{
		VulkanLog("Created a Vulkan Logical Device!")
	}

	//Returns the device queue to be instanced in the "GraphicsQueue"
	vkGetDeviceQueue(VulkanDevice, index.QueueID, 0, &GraphicsQueue);
	//Returns the presentation queue to be instanced in the "PresentationQueue"
	vkGetDeviceQueue(VulkanDevice, index.PresentQueueID, 0, &PresentQueue);
}

void VulkanUtilities::CreateSurface()
{
	VkResult CreatedSurface = glfwCreateWindowSurface(VulkanInstance, GameWindow, nullptr, &VulkanSurface);
	if (CreatedSurface != VK_SUCCESS){
		VulkanFatalLog("Couldn't create a link between the Vulkan api & GLFW!")
	}
	else
	{
		VulkanLog("Created a link between Vulkan & GLFW!")
	}
}

VkPhysicalDevice VulkanUtilities::FindMostSuitableCard(const std::vector<VkPhysicalDevice>& AvailableCards)
{
	typedef std::multimap<int, VkPhysicalDevice> RankedDevicesMap;
	RankedDevicesMap RankedDevices;
	for (VkPhysicalDevice currentDevice : AvailableCards)
	{
		RankedDevices.insert(RankedDevicesMap::value_type(GetDeviceScore(currentDevice), currentDevice));
	}
	if (RankedDevices.rbegin()->first > 0) {
		return RankedDevices.rbegin()->second;
	}
}

int VulkanUtilities::GetDeviceScore(VkPhysicalDevice & Device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(Device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(Device, &deviceFeatures);

	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

const char* VulkanUtilities::GetDeviceName(VkPhysicalDevice & Device)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(Device, &deviceProperties);
	return deviceProperties.deviceName;
}

QueueFamilyIndex VulkanUtilities::GetQueueFamily()
{
	QueueFamilyIndex indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++)
	{
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.QueueID = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, VulkanSurface, &presentSupport);

			if (indices.QueueID >= 0 && presentSupport) {
				indices.PresentQueueID = i;
			}
		}

		if (indices.isComplete()) {
			break;
		}

	}

	return indices;
}

bool VulkanUtilities::isBindedDeviceSuitable()
{
	QueueFamilyIndex index = GetQueueFamily();

	bool SupportsSwapchain = CheckBindedDeviceExtensionSupport();

	//bool swapChainAdequate = false;
	//if (SupportsSwapchain) {
	//	SwapchainSupportDetails swapChainSupport = QuerySwapchainSupport();
	//	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	//}

	return index.isComplete();// && SupportsSwapchain;
}

bool VulkanUtilities::CheckBindedDeviceExtensionSupport()
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

std::vector<const char*> VulkanUtilities::getGLFWExtensions()
{
	std::vector<const char*> extensions;

	uint32_t glfwExtensionCount;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (EnableValidation)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

VulkanUtilities::~VulkanUtilities()
{
	if (EnableValidation)
		VulkanDebug->Cleanup(VulkanInstance);

	vkDestroySurfaceKHR(VulkanInstance, VulkanSurface, nullptr);
	vkDestroyDevice(VulkanDevice, nullptr);
	vkDestroyInstance(VulkanInstance, nullptr);
}
