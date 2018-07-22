#include "GameObject.h"
#include "../Renderer/Model.h"
#include "../Renderer/MeshRenderer.h"
#include "../World/Transform.h"

using namespace LateralEngine;

GameObject::GameObject() {
	ObjectModel = new Model(this);
	transform = new Transform();
}

GameObject::~GameObject() {
	delete(ObjectModel);
	delete(transform);
}

void GameObject::Start() {
}

void GameObject::Tick(float deltaTime) {
}