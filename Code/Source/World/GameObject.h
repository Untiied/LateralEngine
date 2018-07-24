#pragma once
#include "../Renderer/Model.h"
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
		std::shared_ptr<Model> ObjectModel;
		class LateralEngine::Transform* transform;
	};
}
