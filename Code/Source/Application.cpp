#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Log.h"
#include "Window/Window.h"
#include "Engine.h"
#include "World/Cube.h"
#include "World/Chunk.h"
#include "Renderer/Shader.h"
#include "Renderer/OpenglRenderer.h"
#include "Renderer/Camera.h"
#include <stdlib.h>
#include <time.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Utilities/Log.h"
#include "World/ChunkManager.h"
#include <memory>

class AppClass : public Engine
{
	LateralEngine::Rendering::Shader instanceShader;
	LateralEngine::Rendering::ChunkManager chunkManager;
	LateralEngine::Rendering::Camera* camera;
	void init() override {
		Input::window = GameWindow->GetWindow();
		instanceShader.LoadShader("A:/lateralEngine/lateral/Code/Source/Shaders/CubeVertex.vs", "A:/lateralEngine/lateral/Code/Source/Shaders/CubeFrag.fs");
		camera = new LateralEngine::Rendering::Camera(glm::vec3(0, 0, 0), glm::radians(70.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 1000);
		GameWindow->LockCursor();
		srand(time(NULL));
		initImGui();
		chunkManager.GenerateChunks(camera->m_position);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
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
		CalculateDeltaTime();
		ImguiBegin();
		if (MouseLocked) {
			camera->Update();
		}
		camera->freeCamera();
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
		chunkManager.WorldToChunkPosition(camera->m_position);
		chunkManager.Update(camera->m_position);
	}

	void render() override {
		using namespace LateralEngine::Rendering::Opengl;

		OpenglRenderer::GetInstance()->Clear();

		OpenglRenderer::GetInstance()->DrawChunk(&chunkManager, camera, &instanceShader);

		ImGui::Text("Chunk Position: %i, %i", chunkManager.WorldToChunkPosition(camera->m_position).first, chunkManager.WorldToChunkPosition(camera->m_position).second);
		ImGui::Text("Camera Position: %i, %i, %i", (int)round(camera->m_position.x), (int)round(camera->m_position.y), (int)round(camera->m_position.z));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Camera speed: ", &camera->speed, 0, 10);
		if(ImGui::Button("Reset Camera", ImVec2(100, 30)))
		{
			camera->m_position = glm::vec3(0, 16, 0);
		}
		//if (ImGui::Button("test", ImVec2(100, 20)))
		//{
		//	chunkManager.allChunks[0].Move(0, chunkManager.allChunks[0].OneChunk + 2, 0);
		//	chunkManager.UpdateTuple();
		//}

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