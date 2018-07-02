#include "OpenglRenderer.h"
#include "../glad/glad.h"
#include "../Utilities/Globals.h"
#include "../Utilities/Input.h"

using namespace LateralEngine::Rendering::Opengl;
OpenglRenderer* OpenglRenderer::instance = 0;

void OpenglRenderer::Clear()
{
	glClearColor(0.3f, 0.5f, .8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderer::Swap(LateralEngine::Window* window)
{
	glfwSwapBuffers(window->GetWindow());
}

void OpenglRenderer::DrawChunk(LateralEngine::Rendering::Chunk* chunk, LateralEngine::Rendering::Camera* camera, LateralEngine::Rendering::Shader* shader)
{
		for (size_t i = 0; i < chunk->ChunkCubes.size(); i++)
		{
			shader->Bind();
			shader->setMat4("View", camera->GetViewProjection());
			shader->setMat4("Projection", camera->GetPerspective());
			shader->setMat4("Model", chunk->ChunkCubes[i].transform->GetWorldModel());
			shader->setVec3("inColor", chunk->ChunkCubes[i].transform->getColor());
			glBindVertexArray(chunk->ChunkCubes[i].GetBinding());
			glDrawElements(GL_TRIANGLES, chunk->ChunkCubes[i].indices.size(), GL_UNSIGNED_SHORT, 0);
			//glDrawElementsInstanced(GL_TRIANGLES, chunk->ChunkCubes[i].indices.size(), GL_UNSIGNED_SHORT, 0, 10000);
		}
}



void OpenglRenderer::Draw()
{

}

void OpenglRenderer::UpdateViewPort()
{
	glViewport(0, 0, GlobalVariables::Window::width, GlobalVariables::Window::height);
}

