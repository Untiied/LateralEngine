#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanRenderer {
public:
	static VulkanRenderer* instance;

	static VulkanRenderer* GetInstance()
	{
		if (!instance)
			instance = new VulkanRenderer;
		return instance;
	}
	VulkanRenderer();
	//VulkanRenderer(class VulkanUtilities& VulkanUtils);
	~VulkanRenderer();
private:

	//Creates the swapchain for Vulkan to begin the render pipeline.
	void CreateSwapchain();

	//Takes the Swapchain images and creates a "Viewport" that will be used in the render pipeline.
	void CreateImageViews();

	//Creates the actually rendering pipeline.
	void CreateRenderPipeline();

	//Just more structures to make to pass to Vulkan for the ending render pass.
	void CreateRenderPass();

	//Sets up the framebuffers to determine to images
	void CreateFrameBuffers();

	//Sets up the overhead for the command buffer.
	void CreateCommandPool();

	//Sets up the pooling of commands for rendering.
	void CreateCommandBuffers();

	//Checks the front and back for the renderer.
	void CreateSyncObjects();

	//Cleans up the old swapchain.
	void CleanupSwapchain();

	//Populates the Swapchain Details.
	SwapchainSupportDetails QuerySwapchainSupport();

	//Chooses the color format for the render pipeline.
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

	//Choose the present format for the render pipline.
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	//Choose the swap capabilities of the resolution.
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
public:

	//Starts the render processes.
	void Initilize();

	//Recreates the swapchain upon window resizing.
	void RecreateSwapchain();

	//Actually draws the frame.
	void Draw();

protected:
	VkSemaphore ImageAvailableSemaphore;
	VkSemaphore RenderFinishedSemaphore;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkFramebuffer> SwapchainFramebuffers;
	VkCommandPool CommandPool;
	VkPipeline GraphicsPipeline;
	VkPipelineLayout PipelineLayout;
	VkRenderPass RenderPass;
	VkFormat SurfaceFormat;
	VkExtent2D SwapchainExtent;
	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
	VkSwapchainCreateInfoKHR VulkanSwapchainCreateInfo = {};
	VkSwapchainKHR VulkanSwapchain = VK_NULL_HANDLE;
	SwapchainSupportDetails VulkanSwapchainSupport;
	class VulkanUtilities* VulkanBase;
};