#pragma once
#include "Mesh.h"
#include <vector>
#include "Camera.h"
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

		Model(char *path)
		{
			loadModel(path);
		}

		void loadModel(std::string path);
		void LinkMeshes();
		void Draw(Rendering::Camera* camera);
	private:
		/*  Model Data  */
		std::vector<Mesh> meshes;
		std::string directory;
		class GameObject* Owner;
		/*  Functions   */
		void processNode(class aiNode *node, const class aiScene *scene);
		Mesh processMesh(class aiMesh *mesh, const class aiScene *scene);
		//std::vector<Texture> loadMaterialTextures(class aiMaterial *mat, class aiTextureType type, 
		//	class string typeName);
	};
}