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
			std::shared_ptr<Transform> transform = std::make_shared<Transform>();
			std::vector<Cube> ChunkCubes;
			bool visible = true;
		};
	}
}