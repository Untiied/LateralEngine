#pragma once 
#include <vector>
#include <glm/glm.hpp>

namespace LateralEngine {
	struct Vertex;
	class AABB {
	public:
		AABB();
		virtual ~AABB();
	public:
		void CreateBindingBox(class Mesh* mesh);
		inline glm::vec3 getMin() {
			return points[0];
		}		
		inline glm::vec3 getMax() {
			return points[1];
		}
		void Render();
	public:
		glm::vec3 points[2];
	};
}