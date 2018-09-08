#pragma once
#include <string>
#include <map>
#include "../Renderer/Model.h"

namespace LateralEngine {
	static class AssetManager {
	public:
		static void ReloadShaders();

		/* Use to only load a single mesh. Easy to use for debugging purposes. */
		static Mesh DirtyLoadMesh(std::string path);
		
		static std::shared_ptr<Model> LoadModel(std::string path, std::string texpath);
		static std::shared_ptr<Rendering::Texture> LoadTexture(std::string texpath);
		static std::shared_ptr<Rendering::Shader> LoadShader(std::string vertexpath = "", std::string fragmentpath = "");

		static std::map<std::string, std::shared_ptr<Model>> ModelArray;
		static std::map<std::string, std::shared_ptr<Rendering::Texture>> TextureArray;
		static std::map<std::pair<std::string, std::string>, std::shared_ptr<Rendering::Shader>> ShaderArray;
	};
}