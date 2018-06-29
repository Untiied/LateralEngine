#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace GlobalVariables{
	
	namespace Application {
		const extern const char* EngineName;
		extern const uint32_t EngineVersion;
		extern const char* ApplicationName;
	}

	namespace Window {
		extern int width;
		extern int height;
		extern float aspect();
	}

}

namespace Utils {
	extern const std::string currentDateTime();
	extern const std::vector<char> readFile(const char* path);
}