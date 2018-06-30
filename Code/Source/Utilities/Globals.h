#pragma once
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
	extern const std::vector<char> readVulkanShader(const char* path);
	extern const std::string readFile(const char* path);
}