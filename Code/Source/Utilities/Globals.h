#pragma once
#include <string>
#include <vector>
#include <string>
#include "glm/glm.hpp"

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
	extern const float getAspectRatio();
	extern const std::string currentDateTime();
	extern const std::vector<char> readVulkanShader(const char* path);
	extern const std::string readFile(std::string path);

	struct InstanceTuple
	{
		unsigned int vao;
		unsigned int PositionVBO;
		unsigned int ColorVBO;
		unsigned int vbo;
		unsigned int ebo;
		unsigned int amount;
	};
}

namespace Time
{
	extern float deltaTime;
	extern float oldTime;
	extern float newTime;
}