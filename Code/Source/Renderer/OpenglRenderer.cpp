#include "OpenglRenderer.h"
#include "../glad/glad.h"
#include "../Utilities/Input.h"
#include "../World/ChunkManager.h"

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
int test = 0;
void OpenglRenderer::DrawChunk(LateralEngine::Rendering::ChunkManager* chunkManager, LateralEngine::Rendering::Camera* camera, LateralEngine::Rendering::Shader* shader)
{
	if (chunkManager->rankedChunks.size() != 0)
	{
		shader->Bind();
		shader->setMat4("View", camera->GetViewProjection());
		shader->setMat4("Projection", camera->GetPerspective());
		glBindVertexArray(chunkManager->instanceTuple.vao);
		//auto test = chunkManager->rankedChunks.find(std::make_pair(1, 1))->second.ChunkCubes[0].indices.size();
		glDrawElementsInstanced(GL_TRIANGLES,38 , GL_UNSIGNED_SHORT, 0, chunkManager->instanceTuple.amount);
	}
}



void OpenglRenderer::Draw()
{

}

void OpenglRenderer::UpdateViewPort()
{
	glViewport(0, 0, GlobalVariables::Window::width, GlobalVariables::Window::height);
}

Utils::InstanceTuple OpenglRenderer::CreateInstancedCubes(std::vector<glm::mat4> positionData, std::vector<glm::vec3> colorData, LateralEngine::IMesh meshData, float amount)
{

		Utils::InstanceTuple ITuple;

		glGenBuffers(1, &ITuple.PositionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, ITuple.PositionVBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &positionData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &ITuple.ColorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, ITuple.ColorVBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::vec3), &colorData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &ITuple.vao);
		glGenBuffers(1, &ITuple.vbo);
		glGenBuffers(1, &ITuple.ebo);

		glBindVertexArray(ITuple.vao);
		glBindBuffer(GL_ARRAY_BUFFER, ITuple.vbo);
		glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ITuple.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(USHORT), meshData.indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, ITuple.ColorVBO); // this attribute comes from a different vertex buffer
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		// also set instance data
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, ITuple.PositionVBO); // this attribute comes from a different vertex buffer
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
		ITuple.amount = amount;

		return ITuple;
}
