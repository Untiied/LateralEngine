#include "VulkanRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/Globals.h"
#include "Shader.h"
#include <algorithm>

VulkanRenderer *VulkanRenderer::instance = 0;

VulkanRenderer::VulkanRenderer()
{

}

void VulkanRenderer::Initilize()
{
	//Creates the swapchain for Vulkan to begin the render pipeline.
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateRenderPipeline();
	CreateFrameBuffers();
	CreateCommandPool();
	CreateCommandBuffers();
	CreateSyncObjects();
}

void VulkanRenderer::CreateSwapchain() 
{
	//Sets the swapchain creation info.
	SwapchainSupportDetails swapChainSupport = QuerySwapchainSupport();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	
	VulkanSwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	VulkanSwapchainCreateInfo.surface = GetUtilities()->VulkanSurface;
	
	//You have to check the max amount of supported images for the swapchain.
	uint32_t ImageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && ImageCount > swapChainSupport.capabilities.maxImageCount) {
		ImageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VulkanSwapchainCreateInfo.minImageCount = ImageCount;
	VulkanSwapchainCreateInfo.imageFormat = surfaceFormat.format;
	VulkanSwapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	VulkanSwapchainCreateInfo.imageExtent = extent;
	VulkanSwapchainCreateInfo.imageArrayLayers = 1;
	VulkanSwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	//.... used to do a whole setup for the swapchain but aslo differenctiate the difference between present and queue mode.
	VulkanSwapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	VulkanSwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	VulkanSwapchainCreateInfo.presentMode = presentMode;
	VulkanSwapchainCreateInfo.clipped = VK_TRUE;
	//Set this to null because of when you need to reload the swapchain... basically allocate memory for it later.
	VulkanSwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(GetDevice(), &VulkanSwapchainCreateInfo, nullptr, &VulkanSwapchain);

	if (result != VK_SUCCESS) 
	{
		VulkanFatalLog("Failed to create the Vulkan swapchain!")
	}

	vkGetSwapchainImagesKHR(GetDevice(), VulkanSwapchain, &ImageCount, nullptr);
	SwapChainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(GetDevice(), VulkanSwapchain, &ImageCount, SwapChainImages.data());
	
	SwapchainExtent = extent;
	SurfaceFormat = surfaceFormat.format;
}

void VulkanRenderer::CreateImageViews()
{
	SwapChainImageViews.resize(SwapChainImages.size());

	for (size_t i = 0; i < SwapChainImages.size(); i++) {
		//Sets up the swapchain information... for each image ofc.
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = SwapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VulkanSwapchainCreateInfo.imageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(GetDevice(), &createInfo, nullptr, &SwapChainImageViews[i]);
		if(result != VK_SUCCESS)
		{
			VulkanFatalLog("Couldn't create an Vulkan image view for image: %i", i);
		}
	
	}
}

void VulkanRenderer::CreateRenderPass() 
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = SurfaceFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkResult result = vkCreateRenderPass(GetDevice(), &renderPassInfo, nullptr, &RenderPass);
	if(result != VK_SUCCESS)
	{
		VulkanFatalLog("Couldn't create Vulkan render pass!");
	}
}

void VulkanRenderer::CreateRenderPipeline()
{
	//Fake creating a shader just for the Triangle render.
	Shader shader;

	//We start to configure the graphics pipline... 
	//Vertex -> Fragment -> etc.
	VkPipelineShaderStageCreateInfo vertShaderCreationInfo = {};
	vertShaderCreationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderCreationInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderCreationInfo.module = shader.VertexShaderModule;
	vertShaderCreationInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderCreationInfo = {};
	fragShaderCreationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderCreationInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderCreationInfo.module = shader.FragmentShaderModule;
	fragShaderCreationInfo.pName = "main";
	
	//THIS IS A BINDING USED AT THE END OF CREATING THE PIPELINE... YOU WOULD PUT EVER STAGE CREATION IN HERE TEL, RAST, ETC.
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderCreationInfo, fragShaderCreationInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)SwapchainExtent.width;
	viewport.height = (float)SwapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = VulkanSwapchainSupport.capabilities.currentExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	VkResult result = vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &PipelineLayout);
	if(result != VK_SUCCESS)
	{
		VulkanFatalLog("Couldn't create the Vulkan graphics pipeline!")
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = PipelineLayout;
	pipelineInfo.renderPass = RenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VkResult result2 = vkCreateGraphicsPipelines(GetDevice(), NULL , 1, &pipelineInfo, NULL, &GraphicsPipeline);
	if(result2 != VK_SUCCESS)
	{
		VulkanFatalLog("Couldn't create the Vulkan graphics pipeline!")
	}
}

void VulkanRenderer::CreateFrameBuffers()
{
	SwapchainFramebuffers.resize(SwapChainImageViews.size());

	for (size_t i = 0; i < SwapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			SwapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = SwapchainExtent.width;
		framebufferInfo.height = SwapchainExtent.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(GetDevice(), &framebufferInfo, nullptr, &SwapchainFramebuffers[i]);
		if(result != VK_SUCCESS) {
			VulkanFatalLog("Couldn't create the Vulkan frame buffer for image: %n", i);
		}
	}
}

void VulkanRenderer::CreateCommandPool()
{
	QueueFamilyIndex queueFamilyIndices = GetUtilities()->GetQueueFamily();

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.QueueID;
	poolInfo.flags = 0; // Optional

	VkResult result = vkCreateCommandPool(GetDevice(), &poolInfo, nullptr, &CommandPool);
	if (result != VK_SUCCESS)
	{
		VulkanFatalLog("Couldn't create the Vulkan command pool!")
	}
}

void VulkanRenderer::CreateCommandBuffers()
{
	CommandBuffers.resize(SwapchainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)CommandBuffers.size();


	VkResult result = vkAllocateCommandBuffers(GetDevice(), &allocInfo, CommandBuffers.data());
	if (result != VK_SUCCESS) {
		VulkanFatalLog("Couldn't allocate the commands to the Vulkan command buffers!")
	}

	for (size_t i = 0; i < CommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		if (vkBeginCommandBuffer(CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
			VulkanFatalLog("Couldn't begin recording Vulkan command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = RenderPass;
		renderPassInfo.framebuffer = SwapchainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = SwapchainExtent;

		VkClearValue clearColor = { 0.3f, 0.1f, 0.5f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);

		vkCmdDraw(CommandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(CommandBuffers[i]);

		VkResult result = vkEndCommandBuffer(CommandBuffers[i]);
		if (result != VK_SUCCESS) {
			VulkanFatalLog("Couldn't record to the command buffer!")
		}
	}
}

void VulkanRenderer::CreateSyncObjects() 
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &ImageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &RenderFinishedSemaphore) != VK_SUCCESS) {
		VulkanFatalLog("Couldn't create the Vulkan semaphores!");
	}
}

void VulkanRenderer::CleanupSwapchain()
{
	for (auto framebuffer : SwapchainFramebuffers) {
		vkDestroyFramebuffer(GetDevice(), framebuffer, nullptr);
	}

	vkFreeCommandBuffers(GetDevice(), CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());

	vkDestroyPipeline(GetDevice(), GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(GetDevice(), PipelineLayout, nullptr);
	vkDestroyRenderPass(GetDevice(), RenderPass, nullptr);

	for (auto imageView : SwapChainImageViews) {
		vkDestroyImageView(GetDevice(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(GetDevice(), VulkanSwapchain, nullptr);
}

void VulkanRenderer::RecreateSwapchain()
{
	vkDeviceWaitIdle(GetDevice());

	CleanupSwapchain();

	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateRenderPipeline();
	CreateFrameBuffers();
	CreateCommandBuffers();
}

void VulkanRenderer::Draw()
{
	uint32_t imageIndex;
	vkAcquireNextImageKHR(GetDevice(), VulkanSwapchain, std::numeric_limits<uint64_t>::max(), ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { ImageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { RenderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(GetUtilities()->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		VulkanFatalLog("Couldn't submit Vulkan draw command buffers!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { VulkanSwapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(GetUtilities()->PresentQueue, &presentInfo);

	//vkQueueWaitIdle(GetUtilities()->PresentQueue);
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { GlobalVariables::Window::width, GlobalVariables::Window::height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

SwapchainSupportDetails VulkanRenderer::QuerySwapchainSupport()
{
	SwapchainSupportDetails SwapDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GetUtilities()->PhysicalDevice, GetUtilities()->VulkanSurface, &SwapDetails.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(GetUtilities()->PhysicalDevice, GetUtilities()->VulkanSurface, &formatCount, nullptr);

	if (formatCount != 0) {
		SwapDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GetUtilities()->PhysicalDevice, GetUtilities()->VulkanSurface, &formatCount, SwapDetails.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(GetUtilities()->PhysicalDevice, GetUtilities()->VulkanSurface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		SwapDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GetUtilities()->PhysicalDevice, GetUtilities()->VulkanSurface, &presentModeCount, SwapDetails.presentModes.data());
	}

	return SwapDetails;
}

VulkanRenderer::~VulkanRenderer()
{
	vkDestroySemaphore(GetDevice(), RenderFinishedSemaphore, nullptr);
	vkDestroySemaphore(GetDevice(), ImageAvailableSemaphore, nullptr);

	for (auto framebuffer : SwapchainFramebuffers) {
		vkDestroyFramebuffer(GetDevice(), framebuffer, nullptr);
	}

	vkDestroyPipeline(GetDevice(), GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(GetDevice(), PipelineLayout, nullptr);
	vkDestroyRenderPass(GetDevice(), RenderPass, nullptr);

	for (auto imageView : SwapChainImageViews) {
		vkDestroyImageView(GetDevice(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(GetDevice(), VulkanSwapchain, nullptr);
}

