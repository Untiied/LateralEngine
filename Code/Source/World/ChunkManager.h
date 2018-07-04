#pragma once
#include "glm/glm.hpp"
#include "Chunk.h"
#include <vector>
#include <map>
#include "../Renderer/OpenglRenderer.h"

namespace LateralEngine 
{
	namespace Rendering 
	{

		class ChunkManager 
		{
		private:
			//Default 8 chunks
			float ViewDistance = 2;
			const int ChunkSizeX = 16;
			const int ChunkSizeY = 16;
			const int ChunkSizeZ = 16;

			int oldChunkCount = 0;
		public:
			std::vector<glm::vec3> allColor;
			std::vector<glm::mat4> allPositions;

			ChunkManager();

			void GenerateChunks(glm::vec3 position);

			//std::map<std::pair<int, int>, Chunk> allChunks;
			//std::vector<Chunk> allChunks;

			std::map<std::pair<int, int>, Chunk> rankedChunks;

			glm::vec3 ChunkToWorldPosition(std::pair<int, int> ChunkPosition);

			std::pair<int, int> WorldToChunkPositionLie(glm::vec3 position);

			std::pair<int, int> WorldToChunkPosition(glm::vec3 position);

			bool IsChunkInPosition(int x, int z);

			void Update(glm::vec3 position);

			void UpdateTuple();

			inline void UpdateOpenglStuff()
			{
				Cube cube;
				instanceTuple = LateralEngine::Rendering::Opengl::OpenglRenderer::GetInstance()->CreateInstancedCubes(allPositions, allColor, cube, allPositions.size());
			}

			bool IsRenderReady()
			{
				if (oldChunkCount == rankedChunks.size())
				{
					return true;
				}
				return false;
			}

			Utils::InstanceTuple instanceTuple;
		};

	}
}