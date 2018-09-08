#pragma once
#include "Mesh.h"
#include <vector>
#include "Camera.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace LateralEngine {
	class Model {
	public:
		/*  Functions   */
		Model(std::string path, std::string basePath)
		{
			meshes = std::vector<Mesh>();
			loadModel(path, basePath);
		}

		void loadModel(std::string path, std::string texturePath);
		void LinkMeshes();
		void Draw(Rendering::Camera* camera, class GameObject* obj);
		std::vector<Mesh> meshes;
	private:
		/*  Model Data  */
		//Path that is used for textureLoading.
		std::string basePath;
		std::string directory;

		/*  Functions   */
		void processNode(class aiNode *node, const class aiScene *scene);
		Mesh processMesh(class aiMesh *mesh, const class aiScene *scene);
		std::vector<Rendering::Texture> loadMaterialTextures(class aiMaterial *mat, enum aiTextureType type, std::string typeName);
	};
}