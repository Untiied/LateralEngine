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

			 public:
				std::shared_ptr<Transform> transform = std::make_shared<Transform>();
			};
	
	}
}