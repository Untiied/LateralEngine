#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Log.h"
#include "Window/Window.h"
#include "Engine.h"
#include "World/Cube.h"
#include "Renderer/Shader.h"
#include "Renderer/OpenglRenderer.h"
#include "Renderer/Camera.h"

class AppClass : public Engine
{
	std::vector<LateralEngine::Rendering::Cube> CubeScene;
	LateralEngine::Rendering::Camera* camera;
	void init() override {
		Input::window = GameWindow->GetWindow();
		camera = new LateralEngine::Rendering::Camera(glm::vec3(0, 0, 0), glm::radians(90.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 1000);
		GameWindow->LockCursor();
	}

	bool MouseLocked = true;
	void update() override {
		camera->Update();
		camera->freeCamera();
		if (Input::GetKey(KeyCode::ESC)) {
			GameWindow->Close();
		}
		if (Input::GetKeyDown(KeyCode::Q)) {
			LateralEngine::Rendering::Cube cube;
			cube.cubepos = glm::translate(cube.cubepos, camera->m_position);
			cube.GenerateBuffers();
			CubeScene.push_back(cube);
		}
		if (Input::GetKeyDown(KeyCode::F10)) {
			if (MouseLocked) {
				GameWindow->UnlockCursor();
				MouseLocked = false;
			}
			else {
				GameWindow->LockCursor();
				MouseLocked = true;
			}
		}
	}

	void render() override {
		using namespace LateralEngine::Rendering::Opengl;
		OpenglRenderer::GetInstance()->Clear();

		for each (LateralEngine::Rendering::Cube cube in CubeScene)
		{
			cube.shader.Bind();
			cube.shader.setMat4("Model", cube.cubepos);
			cube.shader.setMat4("View", camera->GetViewProjection());
			cube.shader.setMat4("Projection", camera->GetPerspective());
			glBindVertexArray(cube.vao);
			glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_SHORT, 0);
		}

		OpenglRenderer::GetInstance()->Swap(GameWindow);
	}

	void end() override {
		DispenseLog();
	}
};

int main() {
	AppClass app;
	app.start();
}