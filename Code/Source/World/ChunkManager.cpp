#include "ChunkManager.h"
#include "../Utilities/Log.h"
#include "../imgui/imgui.h"
#include "../Renderer/OpenglRenderer.h"
using namespace LateralEngine::Rendering;

ChunkManager::ChunkManager()
{

}

glm::vec3 ChunkManager::ChunkToWorldPosition(std::pair<int, int> ChunkPosition)
{
	float ChunkX = ChunkPosition.first * ChunkSizeX;
	float ChunkZ = ChunkPosition.second * ChunkSizeZ;
	return glm::vec3(ChunkX, 0, ChunkZ);
}

std::pair<int, int> ChunkManager::WorldToChunkPosition(glm::vec3 position)
{

	int ChunkX = (int)floor(position.x) / ChunkSizeX;
	int ChunkZ = (int)floor(position.z) / ChunkSizeZ;

	if (position.x < 0)
	{
		ChunkX += -1;
	}

	if (position.z < 0)
	{
		ChunkZ += -1;
	}

	return std::pair<int, int>(ChunkX, ChunkZ);
}

bool ChunkManager::IsChunkInPosition(int x, int z)
{
	if (rankedChunks.find(std::make_pair(x, z)) == rankedChunks.end())
	{
		return false;
	}
	return true;
}

void ChunkManager::UpdateTuple()
{
	std::vector<glm::vec3> allColor;
	std::vector<glm::mat4> allPositions;
	for (size_t i = 0; i < rankedChunks.size(); i++)
	{
		for(auto it = rankedChunks.begin(); it != rankedChunks.end(); ++it)
		{
			//allPositions.push_back(it->second.transform->GetWorldModel());
			for (int x = 0; x < it->second.ChunkCubes.size(); x++)
			{
				allPositions.push_back(it->second.ChunkCubes[x].transform->GetWorldModel());
				allColor.push_back(it->second.ChunkCubes[x].transform->getColor());
			}
		}
	}

	Cube cube;
	instanceTuple = LateralEngine::Rendering::Opengl::OpenglRenderer::GetInstance()->CreateInstancedCubes(allPositions, allColor, cube, allPositions.size());
}

void ChunkManager::Update(glm::vec3 position)
{
	GenerateChunks(position);
}

int oldChunkCount = 0;
void ChunkManager::GenerateChunks(glm::vec3 position)
{
	using namespace LateralEngine::Rendering;

	auto PlayerToChunk = WorldToChunkPosition(position);

	if (!IsChunkInPosition(PlayerToChunk.first, PlayerToChunk.second))
	{
		Chunk chunk;
		chunk.Move(PlayerToChunk.first * ChunkSizeX, 0, PlayerToChunk.second * ChunkSizeZ);
		rankedChunks.insert(std::make_pair(std::make_pair(PlayerToChunk.first, PlayerToChunk.second), chunk));
	}


	if (oldChunkCount == 0 || oldChunkCount < rankedChunks.size())
	{
		UpdateTuple();
		oldChunkCount = rankedChunks.size();
	}
}

