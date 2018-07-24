#pragma once
#include "Shader.h"

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
			class std::shared_ptr<Rendering::Shader> MeshShader;
			class LateralEngine::Mesh* mesh;
			class LateralEngine::GameObject* Owner;
		};
	}
}