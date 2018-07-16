#include "ChunkManager.h"
#include "../Utilities/Log.h"
#include "../imgui/imgui.h"
#include "../Renderer/OpenglRenderer.h"
#include <future>
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

std::pair<int, int> ChunkManager::WorldToChunkPositionLie(glm::vec3 position)
{

	int ChunkX = (int)floor(position.x) / ChunkSizeX;
	int ChunkZ = (int)floor(position.z) / ChunkSizeZ;

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
	for (size_t i = 0; i < rankedChunks.size(); i++)
	{
		for(auto it = rankedChunks.begin(); it != rankedChunks.end(); ++it)
		{
			if (it->second.updateRender)
			{
				for (int x = 0; x < it->second.ChunkCubes.size(); x++)
				{
					allPositions.push_back(it->second.ChunkCubes[x].transform->GetWorldModel());
					allColor.push_back(it->second.ChunkCubes[x].transform->getColor());
					it->second.updateRender = false;
				}
			}
		}
	}
}

void asyncUpdateTupletest(ChunkManager* ptr)
{
	ptr->UpdateTuple();
}

void ChunkManager::Update(glm::vec3 position)
{
	GenerateChunks(position);
}

void ChunkManager::GenerateChunks(glm::vec3 position)
{
	using namespace LateralEngine::Rendering;

	auto PlayerToChunk = WorldToChunkPosition(position);

	//if (!IsChunkInPosition(PlayerToChunk.first, PlayerToChunk.second))
	//{
	//	Chunk chunk;
	//	chunk.updateRender = true;
	//	chunk.Move(PlayerToChunk.first * ChunkSizeX, 0, PlayerToChunk.second * ChunkSizeZ);
	//	rankedChunks.insert(std::make_pair(std::make_pair(PlayerToChunk.first, PlayerToChunk.second), chunk));
	//}

			std::vector<std::pair<int, int>> pos;
			std::pair<int, int> CurrentChunkPosition = PlayerToChunk;
			int x = 1;
			int z = 1;
			pos.push_back(std::make_pair(CurrentChunkPosition.first + x, CurrentChunkPosition.second - z));
			pos.push_back(std::make_pair(CurrentChunkPosition.first + x, CurrentChunkPosition.second));
			pos.push_back(std::make_pair(CurrentChunkPosition.first + x, CurrentChunkPosition.second + z));
			pos.push_back(std::make_pair(CurrentChunkPosition.first, CurrentChunkPosition.second - z));
			pos.push_back(std::make_pair(CurrentChunkPosition.first, CurrentChunkPosition.second));
			pos.push_back(std::make_pair(CurrentChunkPosition.first, CurrentChunkPosition.second + z));
			pos.push_back(std::make_pair(CurrentChunkPosition.first - x, CurrentChunkPosition.second - z));
			pos.push_back(std::make_pair(CurrentChunkPosition.first - x, CurrentChunkPosition.second));
			pos.push_back(std::make_pair(CurrentChunkPosition.first - x, CurrentChunkPosition.second + z));
	for (int i = 0; i < pos.size(); i++)
	{
		if (!IsChunkInPosition(pos[i].first, pos[i].second))
		{
			Chunk chunk;
			chunk.updateRender = true;
			chunk.Move(pos[i].first * ChunkSizeX, 0, pos[i].second * ChunkSizeZ);
			rankedChunks.insert(std::make_pair(std::make_pair(pos[i].first, pos[i].second), chunk));
		}
	}


	if (oldChunkCount == 0 || oldChunkCount < rankedChunks.size())
	{
		//std::future<void> result(std::async(std::launch::async, asyncUpdateTupletest, this));
		UpdateTuple();
		//result.get();
		updateable = true;
		oldChunkCount = rankedChunks.size();
	}
}

