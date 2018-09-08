#include "../glad/glad.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "../World/GameObject.h"
#include "../World/Transform.h"
#include "../Utilities/AssetManager.h"
using namespace LateralEngine::Rendering;

MeshRenderer::MeshRenderer() {
	MeshShader = AssetManager::LoadShader();
}

MeshRenderer::~MeshRenderer() {
	//delete(MeshShader);
}

void MeshRenderer::BindMesh(LateralEngine::Mesh* mesh) {
			this->mesh = mesh;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(LateralEngine::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned short), mesh->indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LateralEngine::Vertex), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LateralEngine::Vertex), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(LateralEngine::Vertex), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
}

void MeshRenderer::DrawMesh(Camera* camera, LateralEngine::GameObject* obj) {
	MeshShader->Bind();
	MeshShader->setMat4("View", camera->GetViewProjection());
	MeshShader->setMat4("Projection", camera->GetPerspective());
	MeshShader->setMat4("Model", obj->transform->GetWorldModel());

	mesh->material.bindToShader(mesh->meshRenderer.MeshShader);

	if (mesh->meshRenderer.MeshTexture != std::shared_ptr<Texture>()) {
		glActiveTexture(GL_TEXTURE0);
		mesh->meshRenderer.MeshTexture->Bind();
	}
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh->indices.size() , GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);
}
