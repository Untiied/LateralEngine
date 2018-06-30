#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VulkanDebugger.h"

namespace LateralEngine {
	namespace Renderer {
		namespace Vulkan {
			struct QueueFamilyIndex {
				int GraphicsQueue = -1;
				int PresentQueue = -1;

				bool isComplete() {
					return GraphicsQueue >= 0 && PresentQueue >= 0;
				}
			};

			class VulkanResearcher {
			public:
				VulkanResearcher();
				~VulkanResearcher();

				GLFWwindow* window;

				void Initalize();

				//Creates the vulkan instance that controls the overhead.
				void CreateVkInstance();

				//Finds all devices usable by vulkan & picks the best.
				void PickPhyscialDevice();

				//Scores the PhyscialDevices & returns the best one.
				VkPhysicalDevice ReturnBestDevice(std::vector<VkPhysicalDevice> &PhysicalDevices);

				//Creates surface used to find the present queue
				void CreateSurface();

				//Finds the Graphics & Present queues.
				void FindQueues();

				//Creates the interface between Vulkan & Gpu
				void CreateLogicalDevice();

				void Cleanup();

				std::vector<const char*> getGLFWExtensions();

				//Wil be null if you only have 1 graphics card.
				std::vector<VkPhysicalDevice> allDevices;

				inline VkInstance& GetInstance() {
					return vulkanInstance;
				}

				inline VkDevice& GetDevice() {
					return vulkanDevice;
				}

				inline QueueFamilyIndex& GetIndex() {
					return familyIndex;
				}

				inline VkSurfaceKHR& GetSurface() {
					return vulkanSurface;
				}

				inline VkPhysicalDevice& GetPhysicalDevice() {
					return vulkanGPU;
				}

				inline VkQueue& GetGraphicsQueue() {
					return	vulkanGraphicsQueue;
				}

				inline VkQueue& GetPresentQueue() {
					return	vulkanGraphicsQueue;
				}
			private:
				bool EnableValidation;
				VulkanDebugger* VulkanDebug;
				VkInstance vulkanInstance;
				VkPhysicalDevice vulkanGPU;
				VkSurfaceKHR vulkanSurface;
				QueueFamilyIndex familyIndex;
				VkDevice vulkanDevice;
				VkQueue vulkanGraphicsQueue;
				VkQueue vulkanPresentQueue;

				const std::vector<const char*> deviceExtensions = {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			};
		}
	}
}

