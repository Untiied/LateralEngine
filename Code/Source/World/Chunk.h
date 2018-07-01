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

			void GreedyMesh();
		public:
			Transform transform;
			std::vector<Cube> ChunkCubes;
			bool visible = true;
		};
	}
}