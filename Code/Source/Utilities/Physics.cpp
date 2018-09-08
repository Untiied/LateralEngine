#include "Physics.h"
#include "AABB.h"
#include "../World/GameObject.h"
#include "../World/Transform.h"
using namespace LateralEngine;

bool Physics::IsColliding(GameObject & g1, GameObject & g2) {
	AABB* box1 = &g1.ObjectModel->meshes[0].boxCollider;
	AABB* box2 = &g2.ObjectModel->meshes[0].boxCollider;
	box1->points[0] = glm::vec3(g1.transform->getLocation().x - box1->points[0].x, g1.transform->getLocation().y - box1->points[0].y, g1.transform->getLocation().z - box1->points[0].z);
	box1->points[1] = glm::vec3(g1.transform->getLocation().x + box1->points[1].x, g1.transform->getLocation().y + box1->points[1].y, g1.transform->getLocation().z + box1->points[1].z);

	box2->points[0] = glm::vec3(g2.transform->getLocation().x - box2->points[0].x, g2.transform->getLocation().y - box2->points[0].y, g2.transform->getLocation().z - box2->points[0].z);
	box2->points[1] = glm::vec3(g2.transform->getLocation().x + box2->points[1].x, g2.transform->getLocation().y + box2->points[1].y, g2.transform->getLocation().z + box2->points[1].z);

	return (box1->points[0].x <= box2->points[1].x && box1->points[1].x >= box2->points[0].x) &&
			(box1->points[0].y <= box2->points[1].y && box1->points[1].y >= box2->points[0].y) &&
			 (box1->points[0].z <= box2->points[1].z && box1->points[1].z >= box2->points[0].z);

		return false;
}
