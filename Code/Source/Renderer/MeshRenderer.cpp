#include "../glad/glad.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "../World/GameObject.h"
#include "../World/Transform.h"

using namespace LateralEngine::Rendering;

MeshRenderer::MeshRenderer() {
	MeshShader = new Shader();
}

MeshRenderer::~MeshRenderer() {
	//delete(MeshShader);
}

void MeshRenderer::BindMesh(LateralEngine::Mesh* mesh) {
			this->mesh = mesh;
			this->Owner = mesh->Owner;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(LateralEngine::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned short), mesh->indices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LateralEngine::Vertex), (void*)0);
			indexCount = mesh->indices.size();
			glBindVertexArray(0);
}

void MeshRenderer::DrawMesh(Camera* camera) {
	MeshShader->Bind();
	MeshShader->setMat4("View", camera->GetViewProjection());
	MeshShader->setMat4("Projection", camera->GetPerspective());
	MeshShader->setMat4("Model", Owner->transform->GetWorldModel());
	mesh->material.bindToShader(MeshShader);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);
}
