#include "AABB.h"
#include "../Renderer/Mesh.h"
#include "../glad/glad.h"
#include <algorithm>
#include <vector>
#include <numeric>

using namespace LateralEngine;

AABB::AABB() {

}

AABB::~AABB() {

}

void AABB::CreateBindingBox(Mesh* mesh) {
	glm::vec3 lowest = mesh->vertices[0].Position;
	glm::vec3 largest = mesh->vertices[0].Position;
	for (size_t i = 0; i < mesh->vertices.size(); i++) {
		if (mesh->vertices[i].Position.x < lowest.x) {
			if (mesh->vertices[i].Position.y < lowest.y) {
				if (mesh->vertices[i].Position.z < lowest.z) {
					lowest = mesh->vertices[i].Position;
				};
			};
		};
	}
	for (size_t i = 0; i < mesh->vertices.size(); i++) {
		if (mesh->vertices[i].Position.x > largest.x) {
			if (mesh->vertices[i].Position.y > largest.y) {
				if (mesh->vertices[i].Position.z > largest.z) {
					largest = mesh->vertices[i].Position;
				};
			};
		};
	}
	points[0] = lowest;
	points[1] = largest;
}

void AABB::Render() {
	glLoadIdentity();
	glBegin(GL_QUADS);

	glVertex3f(getMin().x, getMin().y, getMax().z);
	glVertex3f(getMax().x, getMin().y, getMax().z);
	glVertex3f(getMax().x, getMax().y, getMax().z);
	glVertex3f(getMin().x, getMax().y, getMax().z);

	glVertex3f(getMax().x, getMin().y, getMax().z);
	glVertex3f(getMax().x, getMin().y, getMin().z);
	glVertex3f(getMax().x, getMax().y, getMin().z);
	glVertex3f(getMax().x, getMax().y, getMax().z);

	glVertex3f(getMin().x, getMax().y, getMax().z);
	glVertex3f(getMax().x, getMax().y, getMax().z);
	glVertex3f(getMax().x, getMax().y, getMin().z);
	glVertex3f(getMin().x, getMax().y, getMin().z);

	glVertex3f(getMin().x, getMin().y, getMin().z);
	glVertex3f(getMin().x, getMax().y, getMin().z);
	glVertex3f(getMax().x, getMax().y, getMin().z);
	glVertex3f(getMax().x, getMin().y, getMin().z);

	glVertex3f(getMin().x, getMin().y, getMin().z);
	glVertex3f(getMax().x, getMin().y, getMin().z);
	glVertex3f(getMax().x, getMin().y, getMax().z);
	glVertex3f(getMin().x, getMin().y, getMax().z);

	glVertex3f(getMin().x, getMin().y, getMin().z);
	glVertex3f(getMin().x, getMin().y, getMax().z);
	glVertex3f(getMin().x, getMax().y, getMax().z);
	glVertex3f(getMin().x, getMax().y, getMin().z);

	glEnd();
}
