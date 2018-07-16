#pragma once
#include "Transform.h"
#include <memory>

namespace LateralEngine {
	namespace Rendering {
		class ChunkCube
		{
		public:
			inline ChunkCube() {};
			inline ~ChunkCube() {};

		public:
			std::shared_ptr<Transform> transform = std::make_shared<Transform>();
		};

	}
}