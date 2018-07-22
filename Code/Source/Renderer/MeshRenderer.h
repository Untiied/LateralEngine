#pragma once

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
			void DrawMesh(class Camera* camera);
			unsigned int vao, vbo, ebo;
			unsigned int indexCount;
		public:
			class Shader* MeshShader;
			class LateralEngine::Mesh* mesh;
			class LateralEngine::GameObject* Owner;
		};
	}
}