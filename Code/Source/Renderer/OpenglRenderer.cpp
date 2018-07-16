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

void OpenglRenderer::Draw()
{

}

void OpenglRenderer::UpdateViewPort()
{
	glViewport(0, 0, GlobalVariables::Window::width, GlobalVariables::Window::height);
}

unsigned long long mat4Size = sizeof(glm::mat4);
unsigned long long vec4Size = sizeof(glm::vec4);
unsigned long long vec3Size = sizeof(glm::vec3);
unsigned long long vertexSize = sizeof(LateralEngine::Vertex);
unsigned long long ushortSize = sizeof(USHORT);
//void OpenglRenderer::CreateInstancedCubes(Utils::InstanceTuple* ITuple, std::vector<glm::mat4>* positionData, std::vector<glm::vec3>* colorData, LateralEngine::IMesh* meshData)
//{
//		glGenBuffers(1, &ITuple->PositionVBO);
//		glBindBuffer(GL_ARRAY_BUFFER, ITuple->PositionVBO);
//		glBufferData(GL_ARRAY_BUFFER, ITuple->amount * mat4Size, positionData->data(), GL_DYNAMIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//
//		glGenBuffers(1, &ITuple->ColorVBO);
//		glBindBuffer(GL_ARRAY_BUFFER, ITuple->ColorVBO);
//		glBufferData(GL_ARRAY_BUFFER, ITuple->amount * vec3Size, colorData->data(), GL_DYNAMIC_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//		glGenVertexArrays(1, &ITuple->vao);
//		glGenBuffers(1, &ITuple->vbo);
//		glGenBuffers(1, &ITuple->ebo);
//
//		glBindVertexArray(ITuple->vao);
//		glBindBuffer(GL_ARRAY_BUFFER, ITuple->vbo);
//		glBufferData(GL_ARRAY_BUFFER, meshData->vertices.size() * vertexSize, meshData->vertices.data(), GL_DYNAMIC_DRAW);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ITuple->ebo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indices.size() * ushortSize, meshData->indices.data(), GL_DYNAMIC_DRAW);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
//
//		glEnableVertexAttribArray(2);
//		glBindBuffer(GL_ARRAY_BUFFER, ITuple->ColorVBO); // this attribute comes from a different vertex buffer
//		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vec3Size, (void*)0);
//
//		// also set instance data
//		glEnableVertexAttribArray(3);
//		glBindBuffer(GL_ARRAY_BUFFER, ITuple->PositionVBO); // this attribute comes from a different vertex buffer
//		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
//		glEnableVertexAttribArray(5);
//		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
//		glEnableVertexAttribArray(6);
//		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
//
//		glVertexAttribDivisor(2, 1);
//		glVertexAttribDivisor(3, 1);
//		glVertexAttribDivisor(4, 1);
//		glVertexAttribDivisor(5, 1);
//		glVertexAttribDivisor(6, 1);
//		glBindVertexArray(0);
//}
