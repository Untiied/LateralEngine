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
			float ViewDistance = 16;
			std::map<std::pair<int, int>, Chunk> allChunks;

			std::pair<int, int> GenerateChunkPosition(glm::vec3 position);
		public:
			void GenerateChunks(glm::vec3 position);
		};

	}
}