#include "Cube.h"
#include "../glad/glad.h"
using namespace LateralEngine::Rendering;

Cube::Cube()
{
	AddVertex(glm::vec3(-0.5, -0.5, 0.5));
	AddVertex(glm::vec3(0.5, -0.5, 0.5));
	AddVertex(glm::vec3(0.5, 0.5, 0.5));
	AddVertex(glm::vec3(-0.5, 0.5, 0.5));

	AddVertex(glm::vec3(-0.5, -0.5, -0.5));
	AddVertex(glm::vec3(0.5, -0.5, -0.5));
	AddVertex(glm::vec3(0.5, 0.5, -0.5));
	AddVertex(glm::vec3(-0.5, 0.5, -0.5));

	std::vector<USHORT> indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};
	AddIndices(indices);
}

//void Cube::GenerateBuffers()
//{
//
//	glGenBuffers(1, &vao);
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);
//
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LateralEngine::Vertex), vertices.data(), GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(USHORT), indices.data(), GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LateralEngine::Vertex), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//	glBindVertexArray(0);
//}

Cube::~Cube()
{
}
