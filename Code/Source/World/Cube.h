#pragma once
#include "../Renderer/IMesh.h"
#include "../Renderer/Shader.h"

namespace LateralEngine {
	namespace Rendering {
			class Cube : public IMesh
			{
			public:
				 Cube();
				 ~Cube();

				 void GenerateBuffers();
			 public:
				Shader shader;
				unsigned int vao, vbo, ebo;
				glm::mat4 cubepos = glm::mat4(1.0f);
			};
	}
}