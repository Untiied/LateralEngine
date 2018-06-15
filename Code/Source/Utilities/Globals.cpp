#include "Globals.h"
#include <time.h>
#include <io.h>
#include <string>
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

	namespace Graphics {
		VkInstance VulkanInstance;
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
}