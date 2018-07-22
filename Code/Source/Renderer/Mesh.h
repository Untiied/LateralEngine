#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include "Texture.h"
#include "Material.h"
#include "MeshRenderer.h"
#define USHORT unsigned short

namespace LateralEngine {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh{
	public:
		Mesh(class GameObject* owner){
			Owner = owner;
		};
		//Add texture cords to this..
		Mesh(std::vector<Vertex> Vertices, std::vector<USHORT> Indices)
			: vertices(Vertices), indices(Indices){};
	public:
		std::vector<Vertex> vertices;
		std::vector<glm::vec2> textureCords;
		std::vector<USHORT> indices;
		Rendering::Material material;
		Rendering::MeshRenderer meshRenderer;
		class GameObject* Owner;
	public:

		inline void AddVertex(glm::vec3 vertice = glm::vec3(0.0f,0.0f,0.0f), 
			glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2 texture = glm::vec2(0.0f, 0.0f)) {

			Vertex funcVer;
			funcVer.Position = vertice;
			funcVer.Normal = normal;
			funcVer.TexCoords = texture;

			vertices.push_back(funcVer);
			textureCords.push_back(texture);
		}
		inline void AddVertex(Vertex vert) {
			vertices.push_back(vert);
		}
		inline void AddIndice(USHORT index) {
			indices.push_back(index);
		}
		inline void AddIndices(std::vector<USHORT> indexes) {
			indices = indexes;
		}
	};

}
