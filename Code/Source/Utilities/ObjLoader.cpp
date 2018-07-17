#pragma once
#include "ObjLoader.h"
#include "../Renderer/Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Log.h"

void ObjLoader::LoadMesh(std::string path, LateralEngine::Mesh * mesh) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
		Log("Couldn't load obj object %s", path.c_str())
	}
	else {
		//std::vector<glm::vec3> Positions;
		//std::vector<glm::vec2> TextureCords;
		//for (int i = 0; i <= attrib.vertices.size(); i+=3) {
		//	if(i != 0)
		//		Positions.push_back(glm::vec3(attrib.vertices[i - 3], attrib.vertices[i - 2], attrib.vertices[i - 1]));
		//}

		//for (int i = 0; i < attrib.texcoords.size(); i+=2) {
		//	if(i != 0)
		//		TextureCords.push_back(glm::vec2(attrib.texcoords[i - 2], attrib.texcoords[i - 1]));
		//}

		//for (auto index : shapes[0].mesh.indices) {
		//	mesh->AddIndice(static_cast<unsigned int>(index.vertex_index));
		//}
	
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				LateralEngine::Vertex vertex;

				vertex.Position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.TexCoords = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				mesh->AddVertex(vertex);
				mesh->AddIndice(static_cast<unsigned int>(index.vertex_index));
			}
		}
	}
}
