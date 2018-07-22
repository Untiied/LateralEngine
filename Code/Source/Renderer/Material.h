#pragma once
#include "glm/glm.hpp"
#include "../glad/glad.h"
#include "Shader.h"

namespace LateralEngine {
	namespace Rendering {
		class Material {
		public:
			inline void bindToShader(Shader* shader) {
				shader->setVec3("material.Ambient", ambient);
			}

			inline Material(glm::vec3 inAmbi = glm::vec3()) {
				ambient = inAmbi;
			};
		public:
			glm::vec3 ambient;
		};
	}
}