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
class AppClass : public Engine
{
	LateralEngine::Rendering::Shader instanceShader;
	std::vector<LateralEngine::Rendering::Chunk> Chunks;
	LateralEngine::Rendering::Camera* camera;
	void init() override {
		Input::window = GameWindow->GetWindow();
		camera = new LateralEngine::Rendering::Camera(glm::vec3(0, 0, 0), glm::radians(90.0f), GlobalVariables::Window::width / GlobalVariables::Window::height, .01f, 1000);
		GameWindow->LockCursor();
		LateralEngine::Rendering::Chunk chunk;
		Chunks.push_back(chunk);
		//glCullFace(GL_FRONT);
		//glEnable(GL_CULL_FACE);
		srand(time(NULL));
	}

	bool MouseLocked = true;
	bool WireFrame = false;
	void update() override {
		camera->Update();
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
	}

	void render() override {
		using namespace LateralEngine::Rendering::Opengl;
		OpenglRenderer::GetInstance()->Clear();

		for each (LateralEngine::Rendering::Chunk chunk in Chunks)
		{
			//for each (LateralEngine::Rendering::Cube cube in chunk.ChunkCubes)
			//{
			//	cube.shader.Bind();
			//	auto test = cube.transform.GetModel();
			//	cube.shader.setMat4("Model", cube.transform.GetModel());
			//	cube.shader.setMat4("View", camera->GetViewProjection());
			//	cube.shader.setMat4("Projection", camera->GetPerspective());
			//	glBindVertexArray(cube.GetBinding());
			//	glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_SHORT, 0);
			//}
			for (size_t i = 0; i < chunk.ChunkCubes.size(); i+=4)
			{
					chunk.ChunkCubes[i].shader.Bind();
					chunk.ChunkCubes[i].shader.setMat4("Model", chunk.ChunkCubes[i].transform.GetModel());
					chunk.ChunkCubes[i].shader.setMat4("View", camera->GetViewProjection());
					chunk.ChunkCubes[i].shader.setMat4("Projection", camera->GetPerspective());
					chunk.ChunkCubes[i].shader.setVec3("inColor", chunk.ChunkCubes[i].transform.Color);
					glBindVertexArray(chunk.ChunkCubes[i].GetBinding());
					glDrawElements(GL_TRIANGLES, chunk.ChunkCubes[i].indices.size(), GL_UNSIGNED_SHORT, 0);
			}
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