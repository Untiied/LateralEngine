#pragma once
#include "../World/Cube.h"
#include "../Renderer/IMesh.h"
#include <vector>

namespace LateralEngine {
	namespace Rendering {
		class Chunk : public IMesh {
		public:
			Chunk();
			~Chunk();
		
			void GenerateChunk();

			std::vector<Cube> ChunkCubes;
		};
	}
}