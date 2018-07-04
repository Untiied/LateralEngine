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
				//allPositions.push_back(it->second.transform->GetWorldModel());
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

	if (!IsChunkInPosition(PlayerToChunk.first, PlayerToChunk.second))
	{
		Chunk chunk;
		chunk.updateRender = true;
		chunk.Move(PlayerToChunk.first * ChunkSizeX, 0, PlayerToChunk.second * ChunkSizeZ);
		rankedChunks.insert(std::make_pair(std::make_pair(PlayerToChunk.first, PlayerToChunk.second), chunk));
	}

	//if (PlayerToChunk.first & 0 && PlayerToChunk.second > 0)
	//{
	//	for (int x = 0; x < PlayerToChunk.first + ViewDistance; x++)
	//	{
	//		for (int z = 0; z < PlayerToChunk.second + ViewDistance; z++)
	//		{
	//			if (!IsChunkInPosition(PlayerToChunk.first + x, PlayerToChunk.second + z))
	//			{
	//				Chunk chunk;
	//				if (PlayerToChunk.first != 0) //|| PlayerToChunk.second != 0)
	//				{
	//					chunk.Move(PlayerToChunk.first * ChunkSizeX * x, 0, PlayerToChunk.second * ChunkSizeZ * z);
	//				}
	//				else
	//				{
	//					chunk.Move(ChunkSizeX * x, 0, ChunkSizeZ * z);
	//				}

	//if (PlayerToChunk.first <= 0 || PlayerToChunk.second <= 0)
	//{
	//	for (int x = 0; x > PlayerToChunk.first - ViewDistance; x--)
	//	{
	//		for (int z = 0; z > PlayerToChunk.second - ViewDistance; z--)
	//		{
	//			if (!IsChunkInPosition(PlayerToChunk.first + x, PlayerToChunk.second + z))
	//			{
	//				Chunk chunk;
	//				if (PlayerToChunk.first != 0) //|| PlayerToChunk.second != 0)
	//				{
	//					chunk.Move(PlayerToChunk.first * ChunkSizeX * x, 0, PlayerToChunk.second * ChunkSizeZ * z);
	//				}
	//				else
	//				{
	//					chunk.Move(ChunkSizeX * x, 0, ChunkSizeZ * z);
	//				}

	if (oldChunkCount == 0 || oldChunkCount < rankedChunks.size())
	{
		//std::future<void> result(std::async(std::launch::async, asyncUpdateTupletest, this));
		UpdateTuple();
		Cube cube;
		oldChunkCount = rankedChunks.size();
		//result.get();
	}
}

