#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

//Unlike opengl you dont need to store the strings... Just upload right to Vulkan.
class Shader {
public:
	Shader();
	~Shader();

	void LoadShader(const char* VertexShaderPath, const char* FragmentShaderPath);

	VkShaderModule CreateShaderModule(std::vector<char>& data);
public:
	VkShaderModule VertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule FragmentShaderModule = VK_NULL_HANDLE;
};