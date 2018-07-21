#pragma once
#include "ObjLoader.h"
#include "../Renderer/Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Log.h"
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "../Renderer/Texture.h"

namespace std {
	template<> struct hash<LateralEngine::Vertex> {
		size_t operator()(LateralEngine::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoords) << 1);
		}
	};
}

void ObjLoader::LoadMesh(std::string path, std::string mtlPath, LateralEngine::Mesh * mesh) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), mtlPath.c_str())) {
		Log("Couldn't load obj object %s", path.c_str())
	}
	else {
		std::unordered_map<LateralEngine::Vertex, uint32_t> uniqueVertices;
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				LateralEngine::Vertex vertex;


				vertex.Position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				if (!attrib.normals.empty()) {
					vertex.Normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (!attrib.texcoords.empty()) {
					vertex.TexCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						/*1.0f -*/ attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(mesh->vertices.size());
					mesh->vertices.push_back(vertex);
				}

				mesh->AddIndice(uniqueVertices[vertex]);
			}
			//shape.mesh.material_ids[0];
		}
		//for (const auto& mat : materials) {
		//	mesh->AddTexture(LateralEngine::Rendering::Texture(mtlPath + mat.name));
		//}
	}
}
