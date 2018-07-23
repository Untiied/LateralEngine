#pragma once
#include "glm/glm.hpp"
#include "../glad/glad.h"
#include "Shader.h"

namespace LateralEngine {
	namespace Rendering {
		class Material {
		public:
			inline void bindToShader(Shader* shader) {
				shader->setVec3("material.Emission", emission);
				shader->setVec3("material.Ambient", ambient);
				shader->setVec3("material.Diffuse", diffuse);
				shader->setFloat("material.Shininess", shininess);
				shader->setVec3("material.Specular", specular);
			}

			inline Material(glm::vec3 inEmission = glm::vec3(), glm::vec3 inAmbi = glm::vec3(), glm::vec3 inDiffuse = glm::vec3(), float inShininess = 0.0f, glm::vec3 inSpecular = glm::vec3()){
				emission = inEmission;
				ambient = inAmbi;
				diffuse = inDiffuse;
				shininess = inShininess;
				specular = inSpecular;
			};
		public:
			glm::vec3 emission;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			float shininess;
			glm::vec3 specular;
		};
	}
}