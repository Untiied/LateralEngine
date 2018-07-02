#include "ChunkManager.h"
#include "../Utilities/Log.h"
#include "../imgui/imgui.h"

using namespace LateralEngine::Rendering;

glm::vec3 ChunkManager::ChunkToWorldPosition(std::pair<int, int> ChunkPosition)
{
	float ChunkX = ChunkPosition.first * ChunkSizeX;
	float ChunkZ = ChunkPosition.second * ChunkSizeZ;

	return glm::vec3(ChunkX, 0, ChunkZ);
}

std::pair<int, int> ChunkManager::WorldToChunkPosition(glm::vec3 position)
{
	int ChunkX = position.x / ChunkSizeX;
	int ChunkZ = position.z / ChunkSizeZ;
	
	return std::pair<int, int>(ChunkX, ChunkZ);
}

void ChunkManager::GenerateChunks(glm::vec3 position)
{
	
}
