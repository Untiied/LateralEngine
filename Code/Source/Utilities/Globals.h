#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace GlobalVariables{
	
	namespace Application {
		const extern const char* EngineName;
		extern const char* ApplicationName;
	}

	namespace Window {
		extern int width;
		extern int height;
		float aspect();
	}

	namespace Graphics {
		//extern VkInstance VulkanInstance;
	}
}

namespace Utils {
	extern const std::string currentDateTime();
}