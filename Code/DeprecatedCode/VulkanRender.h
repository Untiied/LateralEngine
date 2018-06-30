#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Utilities/Vulkan/VulkanResearcher.h"

namespace LateralEngine {
	namespace Renderer {
		namespace Vulkan {
		struct SwapchainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		class VulkanRenderer {
			static VulkanRenderer* instance;
		public:
			static inline VulkanRenderer& GetInstance() {
				if (!instance) {
					instance = new VulkanRenderer();
				}
				return *instance;
			}

			VulkanRenderer();
			~VulkanRenderer();

			void Initilize();

			void CreateCommandPool();

			void CreateCommandBuffer();

			void CreateSwapchain();

			void CreateImageViews();

			void CreateRenderPass();

			void CreateRenderPipeline();

			void CreateFrameBuffers();

			void CreateSemaphores();

			void Render();

			void Cleanup();

			//Populates the Swapchain Details.
			SwapchainSupportDetails QuerySwapchainSupport();

			//Chooses the color format for the render pipeline.
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

			//Choose the present format for the render pipline.
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

			//Choose the swap capabilities of the resolution.
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			inline VulkanResearcher& GetResearcher() {
				return VulkanResearch;
			}

			inline VkInstance& GetVkInstance() {
				return VulkanResearch.GetInstance();
			}

			inline VkDevice& GetDevice() {
				return VulkanResearch.GetDevice();
			}

			inline QueueFamilyIndex& GetIndex() {
				return VulkanResearch.GetIndex();
			}

			inline std::vector<VkCommandBuffer>& GetCommandBuffer() {
				return CommandBuffers;
			}

			inline VkSurfaceKHR& GetSurface() {
				return VulkanResearch.GetSurface();
			}

			inline VkPhysicalDevice& GetPhysicalDevice() {
				return VulkanResearch.GetPhysicalDevice();
			}

			inline VkQueue& GetGraphicsQueue() {
				return VulkanResearch.GetGraphicsQueue();
			}

			inline VkQueue& GetPresentQueue() {
				return	VulkanResearch.GetPresentQueue();
			}
		private:
			VkCommandPool CommandPool;
			VkSwapchainKHR Swapchain;
			VkExtent2D swapchainExtent;
			VkFormat surfaceFormat;
			VkDeviceMemory deviceMemory;
			VkRenderPass renderPass;
			VkPipelineLayout pipelineLayout;
			VkPipeline graphicsPipeline;
			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;
			VulkanResearcher VulkanResearch;

			std::vector<VkCommandBuffer> CommandBuffers;
			std::vector<VkFramebuffer> swapchainFramebuffers;
			std::vector<VkImage> swapchainImages;
			std::vector<VkImageView> swapchainImageViews;
		};
		}
	}
}