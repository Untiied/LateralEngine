#include "GameObject.h"
#include "../Renderer/Model.h"
#include "../Renderer/MeshRenderer.h"
#include "../World/Transform.h"

using namespace LateralEngine;

GameObject::GameObject() {
	transform = new Transform();
}

GameObject::~GameObject() {
	delete(transform);
}

void GameObject::Start() {
}

void GameObject::Tick(float deltaTime) {
}