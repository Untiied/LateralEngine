#include "GameObject.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/MeshRenderer.h"
#include "../World/Transform.h"

using namespace LateralEngine;

GameObject::GameObject() {
	ObjectMesh = new Mesh();
	ObjectRenderer = new Rendering::MeshRenderer();
	transform = new Transform();
	ObjectRenderer->Owner = this;
}

GameObject::~GameObject() {
	delete(ObjectRenderer);
	delete(ObjectMesh);
	delete(transform);
}

void GameObject::Start() {
}

void GameObject::Tick(float deltaTime) {
}

void GameObject::PushMesh() {
	ObjectRenderer->BindMesh(ObjectMesh);
}
