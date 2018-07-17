#pragma once
#include "../Window/Window.h"
#include "Camera.h"
#include <vector>
#include "Mesh.h"
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
