#include "Globals.h"
#include <time.h>
#include <io.h>
#include <string>
#include <fstream>
#include "Log.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Renderer/VulkanRenderer.h"

namespace GlobalVariables {

	namespace Application {
		const const char* EngineName = "Lateral Engine";
		const char* ApplicationName;
	}

	namespace Window {
		 int width = 0;
		 int height = 0;
		 float aspect()
		 {
			 return width/height;
		 }
	}

}

namespace Utils {
	inline const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

		return buf;
	}

	const std::vector<char> readFile(const char * path)
	{	
		std::ifstream inFile(path, std::ios::ate | std::ios::binary);

		if (!inFile.is_open()) {
			Log("Couldn't find shader file: %s", path)
		}

		size_t fileSize = (size_t)inFile.tellg();
		std::vector<char> funcString(fileSize);

		inFile.seekg(0);
		inFile.read(funcString.data(), fileSize);
		inFile.close();

		return funcString;
	}
}