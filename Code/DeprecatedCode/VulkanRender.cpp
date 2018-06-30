#include "VulkanRender.h"
#include "../Utilities/Vulkan/VulkanHelper.h"
#include "../Utilities/Globals.h"
#include "Shader.h"
#include <algorithm>

using namespace LateralEngine::Renderer::Vulkan;
VulkanRenderer *VulkanRenderer::instance = 0;

VulkanRenderer::VulkanRenderer() {

}

void VulkanRenderer::Initilize() {
	VulkanResearch.Initalize();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateRenderPipeline();
	CreateFrameBuffers();
	CreateCommandPool();
	CreateCommandBuffer();
	CreateSemaphores();
}

void VulkanRenderer::CreateCommandPool()
{
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = GetIndex().GraphicsQueue;

	Check(vkCreateCommandPool(GetDevice(), &createInfo, nullptr, &CommandPool));
}

void VulkanRenderer::CreateCommandBuffer()
{
	CommandBuffers.resize(swapchainFramebuffers.size());

	VkCommandBufferAllocateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	createInfo.commandPool = CommandPool;
	createInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	createInfo.commandBufferCount = (uint32_t)CommandBuffers.size();

	Check(vkAllocateCommandBuffers(GetDevice(), &createInfo, &CommandBuffers[0]));

	for (size_t i = 0; i < CommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		Check(vkBeginCommandBuffer(CommandBuffers[i], &beginInfo));

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapchainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapchainExtent;

		VkClearValue clearColor = { 0.3f, 0.1f, 0.5f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		vkCmdDraw(CommandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(CommandBuffers[i]);

		Check(vkEndCommandBuffer(CommandBuffers[i]));
	}
}

void VulkanRenderer::CreateSwapchain()
{
	SwapchainSupportDetails swapChainSupport = QuerySwapchainSupport();
	VkSurfaceFormatKHR surfaceFormatFun = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	//You have to check the max amount of supported images for the swapchain.
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = GetSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormatFun.format;
	createInfo.imageColorSpace = surfaceFormatFun.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	Check(vkCreateSwapchainKHR(GetDevice(), &createInfo, nullptr, &Swapchain));

	vkGetSwapchainImagesKHR(GetDevice(), Swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(GetDevice(), Swapchain, &imageCount, &swapchainImages[0]);

	swapchainExtent = extent;
	surfaceFormat = surfaceFormatFun.format;
}

void VulkanRenderer::CreateImageViews()
{

		//VkImageCreateInfo createInfo = {};
		//createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		//createInfo.imageType = VK_IMAGE_TYPE_2D;
		//createInfo.format = VK_FORMAT_D16_UNORM;
		//createInfo.extent.width = swapchainExtent.width;
		//createInfo.extent.height = swapchainExtent.height;
		//createInfo.extent.depth = 1;
		//createInfo.mipLevels = 1;
		//createInfo.arrayLayers = 1;
		//createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		//createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		//createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		//createInfo.queueFamilyIndexCount = 0;
		//createInfo.pQueueFamilyIndices = nullptr;
		//createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//createInfo.flags = 0;
		//Check(vkCreateImage(GetDevice(), &createInfo, nullptr, &swapchainImages[0]));

		//VkMemoryRequirements deviceMemoryReqs = {};
		//vkGetImageMemoryRequirements(GetDevice(), swapchainImages[i], &deviceMemoryReqs);

		////This might be wrong...
		//VkMemoryAllocateInfo memCreateInfo = {};
		//memCreateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//memCreateInfo.allocationSize = deviceMemoryReqs.size;
		//memCreateInfo.memoryTypeIndex = GetMemoryType(GetPhysicalDevice(), MemoryType::ImageMemory);

		//Check(vkAllocateMemory(GetDevice(), &memCreateInfo, nullptr, &deviceMemory));
		//Check(vkBindImageMemory(GetDevice(), swapchainImages[i], deviceMemory, 0));

		swapchainImageViews.resize(swapchainImages.size());

		for (size_t i = 0; i < swapchainImages.size(); i++) {
		VkImageViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.image = swapchainImages[i];
		viewCreateInfo.format = VK_FORMAT_D16_UNORM;
		viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewCreateInfo.subresourceRange.baseMipLevel = 0;
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.baseArrayLayer = 0;
		viewCreateInfo.subresourceRange.layerCount = 1;
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewCreateInfo.flags = 0;
		Check(vkCreateImageView(GetDevice(), &viewCreateInfo, nullptr, &swapchainImageViews[i]));
	}
}

void VulkanRenderer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = surfaceFormat;
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

	Check(vkCreateRenderPass(GetDevice(), &renderPassInfo, nullptr, &renderPass));
}

void VulkanRenderer::CreateRenderPipeline()
{
	////Fake creating a shader just for the Triangle render.
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
	viewport.width = (float)swapchainExtent.width;
	viewport.height = (float)swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchainExtent;

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

	Check(vkCreatePipelineLayout(GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout));

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
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	Check(vkCreateGraphicsPipelines(GetDevice(), NULL, 1, &pipelineInfo, NULL, &graphicsPipeline));
}

void VulkanRenderer::CreateFrameBuffers()
{
	swapchainFramebuffers.resize(swapchainImageViews.size());

	for (size_t i = 0; i < swapchainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapchainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapchainExtent.width;
		framebufferInfo.height = swapchainExtent.height;
		framebufferInfo.layers = 1;

		Check(vkCreateFramebuffer(GetDevice(), &framebufferInfo, nullptr, &swapchainFramebuffers[i]));
	}
}

void VulkanRenderer::CreateSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	Check(vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore));
	Check(vkCreateSemaphore(GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore));
}

void VulkanRenderer::Render()
{
	//Never handed the cmd to the CommandBuffer....
	uint32_t imageIndex;
	Check(vkAcquireNextImageKHR(GetDevice(), Swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	Check(vkQueueSubmit(GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { Swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	Check(vkQueuePresentKHR(GetPresentQueue(), &presentInfo));
	vkQueueWaitIdle(GetPresentQueue());
}

void VulkanRenderer::Cleanup()
{
	GetResearcher().Cleanup();
}

SwapchainSupportDetails VulkanRenderer::QuerySwapchainSupport()
{
	SwapchainSupportDetails SwapDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GetPhysicalDevice(), GetSurface(), &SwapDetails.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(GetPhysicalDevice(), GetSurface(), &formatCount, nullptr);

	if (formatCount != 0) {
		SwapDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(GetPhysicalDevice(), GetSurface(), &formatCount, SwapDetails.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(GetPhysicalDevice(), GetSurface(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		SwapDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(GetPhysicalDevice(), GetSurface(), &presentModeCount, SwapDetails.presentModes.data());
	}

	return SwapDetails;
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

VulkanRenderer::~VulkanRenderer() {

}