#pragma once
#include "../Renderer/IMesh.h"
#include "../Renderer/Shader.h"
#include "Transform.h"
#include <memory>

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
				std::shared_ptr<Transform> transform = std::make_shared<Transform>();
			private:
				unsigned int vao, vbo, ebo;
			};
	}
}