#include "Chunk.h"
#include "../imgui/imgui.h"
#include "../Renderer/OpenglRenderer.h"
using namespace LateralEngine::Rendering;

Chunk::Chunk()
{
	GenerateChunk();
}

float returnNumber() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Chunk::GenerateChunk()
{
	float test = 0;
	for (size_t x = 0; x < 16; x++)
	{
		for (size_t y = 0; y < 16; y++)
		{
			for (size_t z = 0; z < 16; z++)
			{
				Cube cube;
				cube.transform->setLocation(glm::vec3(x, y, z));
				cube.transform->setColor(glm::vec3(returnNumber(), returnNumber(), returnNumber()));
				cube.transform->SetParent(this->transform);
				ChunkCubes.push_back(cube);
				test++;
			}
		}
	}
}

void Chunk::GreedyMesh()
{
	for each(Cube var in ChunkCubes) {
		for each (Vertex vert in var.vertices) {
			AddVertex(vert);
		}
	}
}

Chunk::~Chunk()
{
}
