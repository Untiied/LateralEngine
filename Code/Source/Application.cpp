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

class AppClass : public Engine
{
	LateralEngine::Rendering::Shader instanceShader;
	std::vector<LateralEngine::Rendering::Chunk> Chunks;
	LateralEngine::Rendering::ChunkManager ChunkMan;
	LateralEngine::Rendering::Camera* camera;
	void init() override {
		Input::window = GameWindow->GetWindow();
		instanceShader.LoadShader("A:/lateralEngine/lateral/Code/Source/Shaders/CubeVertex.vs", "A:/lateralEngine/lateral/Code/Source/Shaders/CubeFrag.fs");
		camera = new LateralEngine::Rendering::Camera(glm::vec3(0, 0, 0), glm::radians(90.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 1000);
		GameWindow->LockCursor();
		LateralEngine::Rendering::Chunk chunk;
		Chunks.push_back(chunk);
		//glCullFace(GL_FRONT);
		//glEnable(GL_CULL_FACE);
		srand(time(NULL));
		initImGui();
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

	bool MouseLocked = true;
	bool WireFrame = false;
	void update() override {
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
		ChunkMan.GenerateChunks(camera->m_position);
	}

	void render() override {
		using namespace LateralEngine::Rendering::Opengl;

		OpenglRenderer::GetInstance()->Clear();

		for (size_t i = 0; i < Chunks.size(); i++)
		{
			OpenglRenderer::GetInstance()->DrawChunk(&Chunks[i], camera, &instanceShader);
		}

		ImGui::Text("Chunk Position: %i, %i", ChunkMan.WorldToChunkPosition(camera->m_position).first, ChunkMan.WorldToChunkPosition(camera->m_position).second);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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