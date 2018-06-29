#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Log.h"
#include "Renderer/VulkanRender.h"
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
	using namespace LateralEngine;
	using namespace LateralEngine::Renderer;
	Window GameWindow;
	VulkanRenderer::GetInstance().GetResearcher().window = GameWindow.GetWindow();
	VulkanRenderer::GetInstance().Initilize();

	while (!GameWindow.ShouldClose()) {
		GameWindow.Update();
		if (Input::GetKey(KeyCode::ESC)) {
			GameWindow.Close();
		}
		VulkanRenderer::GetInstance().Render();
	}

	DispenseLog();
	getchar();
}