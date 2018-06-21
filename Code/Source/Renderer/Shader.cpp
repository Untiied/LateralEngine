#include "Shader.h"
#include "../Utilities/Vulkan/VulkanUtilities.h"
#include "../Utilities/Log.h"

Shader::Shader()
{
	LoadShader("A:/lateralEngine/lateral/Code/Source/Shaders/vert.spv", "A:/lateralEngine/lateral/Code/Source/Shaders/frag.spv");
}

void Shader::LoadShader(const char* VertexShaderPath, const char* FragmentShaderPath)
{
	auto VertexShader = Utils::readFile(VertexShaderPath);
	auto FragmentShader = Utils::readFile(FragmentShaderPath);

	VertexShaderModule = CreateShaderModule(VertexShader);
	FragmentShaderModule = CreateShaderModule(FragmentShader);
}

VkShaderModule Shader::CreateShaderModule(std::vector<char>& data)
{
	VkShaderModule FuncMod;

	VkShaderModuleCreateInfo ShaderCreationInfo = {};
	ShaderCreationInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderCreationInfo.codeSize = data.size();
	ShaderCreationInfo.pCode = reinterpret_cast<uint32_t*>(data.data());

	VkResult result = vkCreateShaderModule(VulkanUtilities::GetInstance()->VulkanDevice, &ShaderCreationInfo, nullptr, &FuncMod);
	if(result != VK_SUCCESS)
	{
		Log("failed to create a Vulkan shader module!")
		return nullptr;
	}

	return FuncMod;
}

Shader::~Shader()
{
	vkDestroyShaderModule(VulkanUtilities::GetInstance()->VulkanDevice, VertexShaderModule, nullptr);
	vkDestroyShaderModule(VulkanUtilities::GetInstance()->VulkanDevice, FragmentShaderModule, nullptr);
}