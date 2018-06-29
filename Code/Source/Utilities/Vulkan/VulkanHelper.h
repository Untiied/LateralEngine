#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <assert.h>
#define Check(result) LateralEngine::CheckResult(result)

namespace LateralEngine {
	
	namespace MemoryType {
		enum MemoryType
		{
			ImageMemory,
			ResourceMemory
		};
	}

	inline void CheckResult(VkResult result) {
		assert(result == VK_SUCCESS);
	}

	inline const char* GetGPUName(VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProps = {};
		vkGetPhysicalDeviceProperties(device, &deviceProps);
		return deviceProps.deviceName;
	}

	inline unsigned int GetGPUMaxMemory(VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProps = {};
		vkGetPhysicalDeviceProperties(device, &deviceProps);
		return deviceProps.limits.maxMemoryAllocationCount;
	}

	inline uint32_t GetMemoryType(VkPhysicalDevice device, MemoryType::MemoryType memoryType) {
		VkPhysicalDeviceMemoryProperties deviceMem;
		vkGetPhysicalDeviceMemoryProperties(device, &deviceMem);

		switch (memoryType) {
		case MemoryType::MemoryType::ImageMemory:
			for (size_t i = 0; i < deviceMem.memoryTypeCount; i++)
			{
				if (deviceMem.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
					return i;
				}
			}
		case MemoryType::MemoryType::ResourceMemory:
			for (size_t i = 0; i < deviceMem.memoryHeapCount; i++)
			{
				if (deviceMem.memoryTypes[i].propertyFlags == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
					return i;
				}
			}
		default:
			return NULL;
		}
	}

}