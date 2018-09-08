#pragma once
#include "Shader.h"
#include "Texture.h"

namespace LateralEngine {
	class Mesh;
	class GameObject;
	namespace Rendering {
		class MeshRenderer{
		public:
			MeshRenderer();
			~MeshRenderer();
		public:
			void BindMesh(LateralEngine::Mesh* mesh);
			void DrawMesh(class Camera* camera, class LateralEngine::GameObject* obj);
			unsigned int vao, vbo, ebo;
			unsigned int indexCount;
		public:
			class std::shared_ptr<Rendering::Texture> MeshTexture;
			class std::shared_ptr<Rendering::Shader> MeshShader;
			class LateralEngine::Mesh* mesh;
			class LateralEngine::GameObject* Owner;

			inline bool HasTexture() {
				if (MeshTexture == std::shared_ptr<Rendering::Texture>()) {
					return false;
				}
				return true;
			}
		};
	}
}