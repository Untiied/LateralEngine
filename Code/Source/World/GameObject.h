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
	public:
		class Model* ObjectModel;
		class LateralEngine::Transform* transform;
	};
}
