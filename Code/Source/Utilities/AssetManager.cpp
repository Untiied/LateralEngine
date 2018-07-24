#include "AssetManager.h"

using namespace LateralEngine;

std::map<std::string, std::shared_ptr<Model>> AssetManager::ModelArray{};
std::map<std::string, std::shared_ptr<Rendering::Texture>> AssetManager::TextureArray{};
std::map<std::string, std::shared_ptr<Rendering::Shader>> AssetManager::ShaderArray{};

void AssetManager::ReloadShaders() {

	//std::map<std::string, std::shared_ptr<Rendering::Shader>>::iterator it = ShaderArray.find(vertexpath);
	//if (it != ShaderArray.end()) {
	//	return it->second;
	//}
	//else {
	//	std::shared_ptr<Rendering::Shader> shader = std::shared_ptr<Rendering::Shader>();
	//	if (vertexpath != "" || fragmentpath != "") {
	//		shader = std::shared_ptr<Rendering::Shader>(new Rendering::Shader(vertexpath, fragmentpath));
	//	}
	//	else {
	//		shader = std::shared_ptr<Rendering::Shader>(new Rendering::Shader());
	//	}
	//	ShaderArray[vertexpath] = shader;
	//	return shader;
	//}

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

std::shared_ptr<Rendering::Texture> LateralEngine::AssetManager::LoadTexture(std::string texpath) {
	std::map<std::string, std::shared_ptr<Rendering::Texture>>::iterator it = TextureArray.find(texpath);
	if (it != TextureArray.end()) {
		return it->second;
	}
	else {
		std::shared_ptr<Rendering::Texture> texture = std::shared_ptr<Rendering::Texture>(new Rendering::Texture(texpath));
		TextureArray[texpath] = texture;
		return texture;
	}
}

std::shared_ptr<Rendering::Shader> LateralEngine::AssetManager::LoadShader(std::string vertexpath, std::string fragmentpath) {
	std::map<std::string, std::shared_ptr<Rendering::Shader>>::iterator it = ShaderArray.find(vertexpath);
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
		}
		ShaderArray[vertexpath] = shader;
		return shader;
	}
}
