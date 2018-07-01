#include "Chunk.h"

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
	transform.setLocation(glm::vec3(100, 10, 1));
	int x = 0;
	int y = 0;
	int z = 0;
	for (size_t x = 0; x < 16; x+=2)
	{
		for (size_t y = 0; y < 16; y+=2)
		{
			for (size_t z = 0; z < 16; z+=2)
			{
				Cube cube;
				cube.GenerateBuffers();
				cube.transform.setLocation(glm::vec3(x, y, z));
				cube.transform.setColor(glm::vec3(returnNumber(), returnNumber(), returnNumber()));
				cube.transform.SetParent(&this->transform);
				ChunkCubes.push_back(cube);
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
	//for each (Cube var in ChunkCubes) {
	//	delete(&var);
	//}
}
