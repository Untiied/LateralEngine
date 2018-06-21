#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Vulkan/VulkanUtilities.h"
#include "Renderer/VulkanRenderer.h"
#include "Utilities/Log.h"
#include "Window/Window.h"

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
	LateralEngine::Window GameWindow;

	VulkanUtilities::GetInstance()->Initilize(*GameWindow.GetWindow());
	VulkanRenderer::GetInstance()->Initilize();
	while (!GameWindow.ShouldClose()) {
		GameWindow.Update();
		if (Input::GetKey(KeyCode::ESC)) {
			GameWindow.Close();
		}
		VulkanRenderer::GetInstance()->Draw();
	}

	delete(VulkanUtilities::GetInstance());
	DispenseLog();
	getchar();
}