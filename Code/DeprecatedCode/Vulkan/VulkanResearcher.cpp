#include "VulkanResearcher.h"
#include "VulkanHelper.h"
#include "../Globals.h"
#include "../Log.h"
#include <algorithm>
#include <set>

using namespace LateralEngine::Renderer::Vulkan;
VulkanResearcher::VulkanResearcher()
{
#ifdef NDEBUG
	EnableValidation = false;
#else
	EnableValidation = true;
	VulkanDebug = new VulkanDebugger;
#endif
}

void VulkanResearcher::Initalize() {
	CreateVkInstance();
	if (EnableValidation)
		VulkanDebug->CreateValidationLayer(GetInstance());
	PickPhyscialDevice();
	CreateSurface();
	FindQueues();
	CreateLogicalDevice();
}

void VulkanResearcher::CreateVkInstance()
{
	auto extensionInfo = getGLFWExtensions();

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pEngineName = GlobalVariables::Application::EngineName;
	applicationInfo.engineVersion = GlobalVariables::Application::EngineVersion;
	applicationInfo.pApplicationName = GlobalVariables::Application::ApplicationName;
	applicationInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &applicationInfo;
	if (EnableValidation) {
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(VulkanDebug->ValidationSize());
		instanceInfo.ppEnabledLayerNames = VulkanDebug->ValidationNames();
	}else{
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;
	}

	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensionInfo.size());
	instanceInfo.ppEnabledExtensionNames = extensionInfo.data();

	Check(vkCreateInstance(&instanceInfo, nullptr, &vulkanInstance));
}

void VulkanResearcher::PickPhyscialDevice()
{
	uint32_t gpuCount;
	std::vector<VkPhysicalDevice> allPhysicalDevices;

	vkEnumeratePhysicalDevices(vulkanInstance, &gpuCount, nullptr);
	allPhysicalDevices.resize(gpuCount);
	vkEnumeratePhysicalDevices(vulkanInstance, &gpuCount, allPhysicalDevices.data());

	vulkanGPU = ReturnBestDevice(allPhysicalDevices);
	VulkanLog(GetGPUName(vulkanGPU));
}
//Not testable unless I can get 2 gpus!
VkPhysicalDevice VulkanResearcher::ReturnBestDevice(std::vector<VkPhysicalDevice>& PhysicalDevices)
{
	if (PhysicalDevices.size() < 2) {
		return PhysicalDevices[0];
	}
	else {
		std::vector<unsigned int> deviceScore;
		allDevices.resize(PhysicalDevices.size());
		for (auto device : PhysicalDevices) {
			allDevices.push_back(device);

			deviceScore.push_back(GetGPUMaxMemory(device));
		}
		auto biggest = std::max_element(std::begin(deviceScore), std::end(deviceScore));
		auto point = std::distance(std::begin(deviceScore), biggest);
		Log("Devices found: %n", deviceScore.size())
		return PhysicalDevices[point];
	}
}

void VulkanResearcher::CreateSurface()
{
	Check(glfwCreateWindowSurface(vulkanInstance, window, nullptr, &vulkanSurface));
}

void VulkanResearcher::FindQueues()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(vulkanGPU, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vulkanGPU, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++)
	{
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			familyIndex.GraphicsQueue = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(vulkanGPU, i, vulkanSurface, &presentSupport);

			if (familyIndex.GraphicsQueue >= 0 && presentSupport) {
				familyIndex.PresentQueue = i;
			}
		}

		if (familyIndex.isComplete()) {
			break;
		}

	}
}

void VulkanResearcher::CreateLogicalDevice()
{
	float QueuePriorities = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { familyIndex.GraphicsQueue,  familyIndex.PresentQueue };
	//Sets up the Queue/Presentation creation information.
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &QueuePriorities;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceFeatures(vulkanGPU, &deviceFeatures);

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (EnableValidation){
		createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanDebug->ValidationSize());
		createInfo.ppEnabledLayerNames = VulkanDebug->ValidationNames();
	}else{
		createInfo.enabledLayerCount = 0;
	}

	Check(vkCreateDevice(vulkanGPU, &createInfo, nullptr, &vulkanDevice));
	vkGetDeviceQueue(GetDevice(), familyIndex.GraphicsQueue, 0, &vulkanGraphicsQueue);
	vkGetDeviceQueue(GetDevice(), familyIndex.PresentQueue, 0, &vulkanPresentQueue);
}

void VulkanResearcher::Cleanup() {

	vkDestroyDevice(vulkanDevice, nullptr);
	vkDestroySurfaceKHR(vulkanInstance, vulkanSurface, nullptr);
	vkDestroyInstance(vulkanInstance, nullptr);
}

std::vector<const char*> VulkanResearcher::getGLFWExtensions()
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

VulkanResearcher::~VulkanResearcher()
{

}

