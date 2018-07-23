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
		Model(class GameObject* owner) {
			Owner = owner;
		}

		Model(char *path, char *basePath)
		{
			loadModel(path, basePath);
		}

		void loadModel(std::string path, std::string texturePath);
		void LinkMeshes();
		void Draw(Rendering::Camera* camera);
	private:
		/*  Model Data  */
		//Path that is used for textureLoading.
		std::string basePath;
		std::vector<Mesh> meshes;
		std::vector<Rendering::Texture> textures;
		std::string directory;
		class GameObject* Owner;
		/*  Functions   */
		void processNode(class aiNode *node, const class aiScene *scene);
		Mesh processMesh(class aiMesh *mesh, const class aiScene *scene);
		std::vector<Rendering::Texture> loadMaterialTextures(class aiMaterial *mat, enum aiTextureType type, 
			std::string typeName);
	};
}