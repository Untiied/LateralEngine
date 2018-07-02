#pragma once
#include "glm/glm.hpp"
#include "Chunk.h"
#include <vector>
#include <map>

namespace LateralEngine 
{
	namespace Rendering 
	{

		class ChunkManager 
		{
		private:
			//Default 16 chunks
			float ViewDistance = 8;
			const int ChunkSizeX = 16;
			const int ChunkSizeY = 16;
			const int ChunkSizeZ = 16;

			std::map<std::pair<int, int>, Chunk> allChunks;

		public:
			glm::vec3 ChunkToWorldPosition(std::pair<int, int> ChunkPosition);

			std::pair<int, int> WorldToChunkPosition(glm::vec3 position);

			void GenerateChunks(glm::vec3 position);
		};

	}
}