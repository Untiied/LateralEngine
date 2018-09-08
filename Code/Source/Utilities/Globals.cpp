#include "Globals.h"
#include <time.h>
#include <io.h>
#include <string>
#include <fstream>
#include "Log.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace GlobalVariables {

	namespace Application {
		const const char* EngineName = "Lateral Engine";
		const uint32_t EngineVersion = (0, 0, 2);
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
	const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

		return buf;
	}

	const std::vector<char> readVulkanShader(const char * path)
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

	const std::string readFile(std::string path)
	{
			if(path.find('/') == std::string::npos )
			{
				Log("Reading file: %s uses '/'... Terminated search", path.c_str());
				return std::string();
			}
		std::string s(std::istreambuf_iterator<char>(std::ifstream(path)
			>> std::skipws),
			std::istreambuf_iterator<char>());

		return s;
	}

	const float getAspectRatio() {
		return  GlobalVariables::Window::width / GlobalVariables::Window::height;
	}
}

namespace Time
{
	float deltaTime = 0;
	float oldTime = 0;
	float newTime = 0;
}