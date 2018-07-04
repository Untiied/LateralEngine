#pragma once
#include "../World/Cube.h"
#include "../Renderer/IMesh.h"
#include <vector>
#include "../Utilities/Globals.h"
namespace LateralEngine {
	namespace Rendering {

		class Chunk : public IMesh {
		public:
			Chunk();
			~Chunk();
		
			void GenerateChunk();

			void GreedyMesh();

			inline void Move(float x, float y, float z)
			{
				transform->addLocation(glm::vec3(x, y, z));
			}
		public:
			std::shared_ptr<Transform> transform = std::make_shared<Transform>();
			std::vector<Cube> ChunkCubes;
			bool visible = true;
			bool updateRender = false;
			float OneChunk = 16;
			glm::vec2 ChunkFormationPosition;
		};
	}
}