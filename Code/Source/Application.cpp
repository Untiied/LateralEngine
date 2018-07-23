#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Log.h"
#include "Window/Window.h"
#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/OpenglRenderer.h"
#include "Renderer/Camera.h"
#include <stdlib.h>
#include <time.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Utilities/Log.h"
#include "World/GameObject.h"
#include "Renderer/MeshRenderer.h"
#include <memory>
#include <thread>
#include "Renderer/Texture.h"
#include "Renderer/Model.h"

class AppClass : public Engine
{
	LateralEngine::Rendering::Camera* camera;
	LateralEngine::Rendering::Texture* texture;
	LateralEngine::GameObject obj;
	void init() override {
		Input::window = GameWindow->GetWindow();
		camera = new LateralEngine::Rendering::Camera(glm::vec3(0, 0, 0), glm::radians(90.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 6000);
		GameWindow->LockCursor();
		initImGui();
		glEnable(GL_DEPTH_TEST);
		obj.ObjectModel->loadModel("A:/MODELS/sponza/sponza.obj","A:/MODELS/sponza/");
	}

	void initImGui() {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfw_InitForOpenGL(GameWindow->GetWindow(), false);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();
	}
	void ImguiBegin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImguiEnd() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void CalculateDeltaTime()
	{
		Time::oldTime = Time::newTime;
		Time::newTime = glfwGetTime();
		Time::deltaTime = Time::newTime - Time::oldTime;
	}
	bool MouseLocked = true;
	bool WireFrame = false;

	void update() override {
		ImguiBegin();
		CalculateDeltaTime();
		camera->freeCamera();
		if (MouseLocked) {
			camera->Update();
		}
		if (Input::GetKey(KeyCode::ESC)) {
			GameWindow->Close();
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
		if (Input::GetKeyDown(KeyCode::F11)) 
		{
			if(!WireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				WireFrame = true;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				WireFrame = false;
			}
		}
	}

	void render() override {
		using namespace LateralEngine::Rendering::Opengl;
		OpenglRenderer::GetInstance()->Clear();
		obj.ObjectModel->Draw(camera);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera Positions %f, %f, %f", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		ImGui::SliderFloat("Camera speed: ", &camera->speed, 0, 1000);
		ImguiEnd();
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