#include "Model.h"
#include "../Utilities/Log.h"
using namespace LateralEngine;

void Model::loadModel(std::string path, std::string texturePath) {
	basePath = texturePath;
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		Log("ERROR::ASSIMP::%s", import.GetErrorString())
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
	LinkMeshes();
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
		
		if (mesh->HasTextureCoords(0)) {
			vertex.TexCoords.x = mesh->mTextureCoords[0][v].x; 
			vertex.TexCoords.y = mesh->mTextureCoords[0][v].y;
		}

		//if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		//{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).

		//}

		returnedMesh.AddVertex(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			returnedMesh.AddIndice(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString path;
		std::string beforePath = basePath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			beforePath = beforePath + path.C_Str();
			returnedMesh.texture.LoadTexture(beforePath);
		}

		aiColor3D color (0.f,0.f,0.f);
		material->Get(AI_MATKEY_COLOR_EMISSIVE,color);
		glm::vec3 matColor{
			color.r,
			color.g,
			color.b
		};
		returnedMesh.material.emission = matColor;

		material->Get(AI_MATKEY_COLOR_AMBIENT,color);
		glm::vec3 mat1Color{
			color.r,
			color.g,
			color.b
		};
		returnedMesh.material.ambient = mat1Color;

		material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
		glm::vec3 mat2Color{
			color.r,
			color.g,
			color.b
		};
		returnedMesh.material.diffuse = mat2Color;

		//float shine;
		//material->Get(AI_MATKEY_UVTRANSFORM_SHININESS,color);
		//glm::vec3 matColor{
		//	color.r,
		//	color.g,
		//	color.b
		//};
		//returnedMesh.material.shininess = matColor;

		material->Get(AI_MATKEY_COLOR_SPECULAR,color);
		glm::vec3 mat3Color{
			color.r,
			color.g,
			color.b
		};
		returnedMesh.material.specular = mat3Color;

		//// 1. diffuse maps
		//std::vector<Rendering::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//// 2. specular maps
		//std::vector<Rendering::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//// 3. normal maps
		//std::vector<Rendering::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//// 4. height maps
		//std::vector<Rendering::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	return returnedMesh;
}

std::vector<Rendering::Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName) {
	std::vector<Rendering::Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
			aiString str;
			mat->GetTexture(type, i, &str);
			// if texture hasn't been loaded already, load it
			Rendering::Texture texture(str.C_Str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			//textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}
	return textures;
}
