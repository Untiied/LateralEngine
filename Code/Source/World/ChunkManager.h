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
			float ViewDistance = 8;
			const int ChunkSizeX = 16;
			const int ChunkSizeY = 16;
			const int ChunkSizeZ = 16;

			int oldChunkCount = 0;
			bool updateable = false;
		public:
			std::vector<glm::vec3> allColor;
			std::vector<glm::mat4> allPositions;

			ChunkManager();

			void GenerateChunks(glm::vec3 position);

			std::map<std::pair<int, int>, Chunk> rankedChunks;

			glm::vec3 ChunkToWorldPosition(std::pair<int, int> ChunkPosition);

			std::pair<int, int> WorldToChunkPositionLie(glm::vec3 position);

			std::pair<int, int> WorldToChunkPosition(glm::vec3 position);

			bool IsChunkInPosition(int x, int z);

			void Update(glm::vec3 position);

			void UpdateTuple();

			Cube cube;
			inline void UpdateOpenglStuff()
			{
				instanceTuple.amount = allPositions.size();
				LateralEngine::Rendering::Opengl::OpenglRenderer::GetInstance()->ReloadInstancedCubes(&instanceTuple, &allPositions, &allColor, &cube);
				updateable = false;
			}

			inline void FirstOpengl()
			{
				instanceTuple.amount = allPositions.size();
				LateralEngine::Rendering::Opengl::OpenglRenderer::GetInstance()->CreateInstancedCubes(&instanceTuple, &allPositions, &allColor, &cube);
				updateable = false;
			}

			inline bool IsRenderReady()
			{
				if (oldChunkCount == rankedChunks.size())
				{
					return true;
				}
				return false;
			}

			inline bool IsRenderUpdateReady()
			{
				if (updateable == true)
				{
					return true;
				}
				return false;
			}

			inline int GetChunkAmount()
			{
				return rankedChunks.size();
			}

			Utils::InstanceTuple instanceTuple;
		};

	}
}