#include "AssetManager.h"

using namespace LateralEngine;

std::map<std::string, std::shared_ptr<Model>> AssetManager::ModelArray{};
std::map<std::string, std::shared_ptr<Rendering::Texture>> AssetManager::TextureArray{};
std::map<std::pair<std::string, std::string>, std::shared_ptr<Rendering::Shader>> AssetManager::ShaderArray{};
void AssetManager::ReloadShaders() {

	std::map<std::pair<std::string, std::string>, std::shared_ptr<Rendering::Shader>>::iterator it;
	for (it = ShaderArray.begin(); it != ShaderArray.end(); it++) {
		it->second->LoadShader(it->first.first, it->first.second);
	}
}

std::shared_ptr<Model> AssetManager::LoadModel(std::string path, std::string texpath) {
	std::map<std::string, std::shared_ptr<Model>>::iterator it = ModelArray.find(path);
	if (it != ModelArray.end()) {
		return it->second;
	}
	else {
		std::shared_ptr<Model> model = std::shared_ptr<Model>(new Model(path, texpath));
		ModelArray[path] = model;
		return model;
	}
}

LateralEngine::Mesh LateralEngine::AssetManager::DirtyLoadMesh(std::string path) {
	LateralEngine::Model* funcModel = new LateralEngine::Model(path,"");
	return funcModel->meshes[0];
}

std::shared_ptr<Rendering::Texture> LateralEngine::AssetManager::LoadTexture(std::string texpath) {
	std::map<std::string, std::shared_ptr<Rendering::Texture>>::iterator it = TextureArray.find(texpath);
	if (it != TextureArray.end()) {
		return it->second;
	}
	else {
		std::shared_ptr<Rendering::Texture> texture = std::shared_ptr<Rendering::Texture>();
		texture = std::shared_ptr<Rendering::Texture>(new Rendering::Texture(texpath));
		TextureArray[texpath] = texture;
		return texture;
	}
}

std::shared_ptr<Rendering::Shader> LateralEngine::AssetManager::LoadShader(std::string vertexpath, std::string fragmentpath) {
	std::map<std::pair<std::string, std::string>, std::shared_ptr<Rendering::Shader>>::iterator it = ShaderArray.find(std::make_pair(vertexpath,fragmentpath));
	if (it != ShaderArray.end()) {
		return it->second;
	}
	else {
		std::shared_ptr<Rendering::Shader> shader = std::shared_ptr<Rendering::Shader>();
		if (vertexpath != "" || fragmentpath != "") {
			shader = std::shared_ptr<Rendering::Shader>(new Rendering::Shader(vertexpath, fragmentpath));
		}
		else {
			shader = std::shared_ptr<Rendering::Shader>(new Rendering::Shader());
			vertexpath = "A:/lateralEngine/lateral/Code/Source/Shaders/BasicVertex.vs";
			fragmentpath = "A:/lateralEngine/lateral/Code/Source/Shaders/BasicFrag.fs";
		}
		ShaderArray[std::make_pair(vertexpath,fragmentpath)] = shader;
		return shader;
	}
}
