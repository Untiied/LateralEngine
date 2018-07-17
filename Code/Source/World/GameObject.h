#pragma once

namespace LateralEngine {
	class Transform;
	namespace Rendering {
		class MeshRenderer;
	}
	class GameObject {
	public:
		GameObject();
		~GameObject();

	public:
		virtual void Start();
		virtual void Tick(float deltaTime);
		void PushMesh();
	public:
		class Mesh* ObjectMesh;
		class Rendering::MeshRenderer* ObjectRenderer;
		class LateralEngine::Transform* transform;
	};
}
