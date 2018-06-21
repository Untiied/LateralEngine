#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

//Unlike opengl you dont need to store the strings... Just upload right to Vulkan.
class Shader {
public:
	Shader(class VulkanUtilities& vkUtils);
	~Shader();

	void LoadShader(class VulkanUtilities& vkUtils, const char* VertexShaderPath, const char* FragmentShaderPath);

	VkShaderModule CreateShaderModule(class VulkanUtilities& vkUtils, std::vector<char>& data);
public:
	class VulkanUtilities* VulkanUtils;
	VkShaderModule VertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule FragmentShaderModule = VK_NULL_HANDLE;
};