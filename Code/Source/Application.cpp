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
#include "Utilities/ObjLoader.h"
#include <memory>
#include <thread>
#include "Utilities/tiny_obj_loader.h"

class AppClass : public Engine
{
	LateralEngine::Rendering::Camera* camera;
	LateralEngine::GameObject obj;
	void init() override {
		Input::window = GameWindow->GetWindow();
		camera = new LateralEngine::Rendering::Camera(glm::vec3(-5, 0, 0), glm::radians(70.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 1000);
		GameWindow->LockCursor();
		srand(time(NULL));
		initImGui();
		glEnable(GL_DEPTH_TEST);
		//std::vector<glm::vec3> cube_vertices = {
		//	// front
		//	glm::vec3(-1.0, -1.0,  1.0),
		//	glm::vec3(1.0, -1.0,  1.0),
		//	glm::vec3(1.0,  1.0,  1.0),
		//	glm::vec3(-1.0,  1.0,  1.0),
		//	// back
		//	glm::vec3(-1.0, -1.0, -1.0),
		//	glm::vec3(1.0, -1.0, -1.0),
		//	glm::vec3(1.0,  1.0, -1.0),
		//	glm::vec3(-1.0,  1.0, -1.0)
		//};
		//std::vector<unsigned short> cube_elements = {
		//	// front
		//	0, 1, 2,
		//	2, 3, 0,
		//	// right
		//	1, 5, 6,
		//	6, 2, 1,
		//	// back
		//	7, 6, 5,
		//	5, 4, 7,
		//	// left
		//	4, 0, 3,
		//	3, 7, 4,
		//	// bottom
		//	4, 5, 1,
		//	1, 0, 4,
		//	// top
		//	3, 2, 6,
		//	6, 7, 3,
		//};
		//for (size_t i = 0; i < cube_vertices.size(); i++) {
		//	obj.ObjectMesh->AddVertex(cube_vertices[i]);
		//}
		//obj.ObjectMesh->indices = cube_elements;
		ObjLoader::LoadMesh("A:/MODELS/sibenik/sibenik.obj", obj.ObjectMesh);
		obj.PushMesh();
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
		obj.ObjectRenderer->DrawMesh(camera);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Camera Positions %f, %f, %f", camera->m_position.x, camera->m_position.y, camera->m_position.z);
		ImGui::SliderFloat("Camera speed: ", &camera->speed, 0, 100);
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