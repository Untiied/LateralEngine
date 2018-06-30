#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <vector>

#define USHORT unsigned short
namespace LateralEngine {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class IMesh{
	public:
		IMesh(){};

		IMesh(std::vector<Vertex> Vertices, std::vector<USHORT> Indices)
			: vertices(Vertices), indices(Indices){};
	public:
		std::vector<Vertex> vertices;
		std::vector<USHORT> indices;
	public:
		inline void AddVertex(glm::vec3 vertice = glm::vec3(0.0f,0.0f,0.0f), 
			glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2 texture = glm::vec2(0.0f, 0.0f)) {

			Vertex funcVer;
			funcVer.Position = vertice;
			funcVer.Normal = normal;
			funcVer.TexCoords = texture;

			vertices.push_back(funcVer);
		}

		inline void AddIndice(USHORT index) {
			indices.push_back(index);
		}
		inline void AddIndices(std::vector<USHORT> indexes) {
			indices = indexes;
		}
	};

}
