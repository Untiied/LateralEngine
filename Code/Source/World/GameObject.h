#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "../Renderer/Model.h"
#include "Transform.h"
#include "../Renderer/MeshRenderer.h"
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
		LateralEngine::Transform* transform;
	};
}
#endif