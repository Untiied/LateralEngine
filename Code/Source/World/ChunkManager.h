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
			//Default 8 chunks
			float ViewDistance = 8;
			const int ChunkSizeX = 16;
			const int ChunkSizeY = 16;
			const int ChunkSizeZ = 16;


		public:
			ChunkManager();

			void GenerateChunks(glm::vec3 position);

			//std::map<std::pair<int, int>, Chunk> allChunks;
			//std::vector<Chunk> allChunks;

			std::map<std::pair<int, int>, Chunk> rankedChunks;

			glm::vec3 ChunkToWorldPosition(std::pair<int, int> ChunkPosition);

			std::pair<int, int> WorldToChunkPosition(glm::vec3 position);

			bool IsChunkInPosition(int x, int z);

			void Update(glm::vec3 position);

			void UpdateTuple();

			Utils::InstanceTuple instanceTuple;
		};

	}
}