#include "Model.h"
#include "../Utilities/Log.h"
using namespace LateralEngine;

void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		Log("ERROR::ASSIMP::%s", import.GetErrorString())
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::LinkMeshes() {
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i].meshRenderer.BindMesh(&meshes[i]);
	}
}

void Model::Draw(Rendering::Camera* camera) {
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i].meshRenderer.DrawMesh(camera);
	}
}

void Model::processNode(aiNode * node, const aiScene * scene) {
	// process all the node's meshes (if any)
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
		meshes.push_back(processMesh(mesh, scene));			
	}
	// then do the same for each of its children
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene) {
	Mesh returnedMesh(Owner);

	for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[v].x;
		vertex.Position.y = mesh->mVertices[v].y;
		vertex.Position.z = mesh->mVertices[v].z; 

		if (mesh->HasNormals()) {
			vertex.Normal.x = mesh->mNormals[v].x;
			vertex.Normal.y = mesh->mNormals[v].y;
			vertex.Normal.z = mesh->mNormals[v].z;
		}

		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.TexCoords.x = mesh->mTextureCoords[0][v].x; 
			vertex.TexCoords.y = mesh->mTextureCoords[0][v].y;
		}

		returnedMesh.AddVertex(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			returnedMesh.AddIndice(face.mIndices[j]);
	}

	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	aiColor3D color (0.f,0.f,0.f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE,color);
	glm::vec3 matColor{
		color.r,
		color.g,
		color.b
	};
	returnedMesh.material = Rendering::Material(matColor);
	return returnedMesh;
}
