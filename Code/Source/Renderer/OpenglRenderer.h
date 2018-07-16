#pragma once
#include "../Window/Window.h"
#include "Camera.h"
#include <vector>
#include "IMesh.h"
#include "glm/glm.hpp"
#include "../Utilities/Globals.h"

namespace LateralEngine {
	namespace Rendering {
		class ChunkManager;
		namespace Opengl {
			class OpenglRenderer {
			public:
				void Clear();
				void Swap(LateralEngine::Window* window);
				void Draw();
				void UpdateViewPort();

				//Utils::InstanceTuple CreateInstancedCubes(std::vector<glm::mat4> positionData, std::vector<glm::vec3> colorData, LateralEngine::IMesh meshData, float amount);

				static OpenglRenderer* GetInstance()
				{
					if (!instance)
					{
						instance = new OpenglRenderer();
					}
					return instance;
				}
			private:
				static OpenglRenderer* instance;
			};
		}
	}
}
