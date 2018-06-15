#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Utilities/Input.h"
#include "Window/Window.h"
#include "Utilities/Globals.h"
#include "Utilities/Vulkan/VulkanUtilities.h"
#include "Utilities/Log.h"

#if 0
class Engine
{
	VulkanUtilities VkUtils;
public:
	inline Engine();
	inline ~Engine() { delete(&VkUtils); };


private:

};
#endif

int main() {
	VulkanUtilities VkUtils;
	LateralEngine::Window GameWindow;

	VkUtils.Initilize(*GameWindow.GetWindow());

	while (!GameWindow.ShouldClose()) {
		GameWindow.Update();
		if (Input::GetKey(KeyCode::ESC)) {
			GameWindow.Close();
		}
	}

	//delete(&VkUtils);
	DispenseLog();
}