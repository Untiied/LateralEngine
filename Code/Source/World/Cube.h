#pragma once
#include "../Renderer/IMesh.h"
#include "../Renderer/Shader.h"
#include "Transform.h"

namespace LateralEngine {
	namespace Rendering {
			class Cube : public IMesh
			{
			public:
				 Cube();
				 ~Cube();

				 void GenerateBuffers();
				 inline unsigned int GetBinding() {
					 return vao;
				 }
			 public:
				Shader shader;
				Transform transform;
			private:
				unsigned int vao, vbo, ebo;
			};
	}
}