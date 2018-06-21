#include "Shader.h"
#include "../Utilities/Vulkan/VulkanUtilities.h"
#include "../Utilities/Log.h"

Shader::Shader(VulkanUtilities& vkUtils)
{
	LoadShader(vkUtils, "A:/lateralEngine/lateral/Code/Source/Shaders/vert.spv", "A:/lateralEngine/lateral/Code/Source/Shaders/frag.spv");
	VulkanUtils = &vkUtils;
}

void Shader::LoadShader(VulkanUtilities& vkUtils, const char* VertexShaderPath, const char* FragmentShaderPath)
{
	auto VertexShader = Utils::readFile(VertexShaderPath);
	auto FragmentShader = Utils::readFile(FragmentShaderPath);

	VertexShaderModule = CreateShaderModule(vkUtils, VertexShader);
	FragmentShaderModule = CreateShaderModule(vkUtils, FragmentShader);
}

VkShaderModule Shader::CreateShaderModule(VulkanUtilities& vkUtils, std::vector<char>& data)
{
	VkShaderModule FuncMod;

	VkShaderModuleCreateInfo ShaderCreationInfo = {};
	ShaderCreationInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderCreationInfo.codeSize = data.size();
	ShaderCreationInfo.pCode = reinterpret_cast<uint32_t*>(data.data());

	VkResult result = vkCreateShaderModule(vkUtils.VulkanDevice, &ShaderCreationInfo, nullptr, &FuncMod);
	if(result != VK_SUCCESS)
	{
		Log("failed to create a Vulkan shader module!")
		return nullptr;
	}

	return FuncMod;
}

Shader::~Shader()
{
	vkDestroyShaderModule(VulkanUtils->VulkanDevice, VertexShaderModule, nullptr);
	vkDestroyShaderModule(VulkanUtils->VulkanDevice, FragmentShaderModule, nullptr);
}