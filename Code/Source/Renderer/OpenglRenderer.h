#pragma once
#include "../Window/Window.h"
#include "../World/Chunk.h"
#include "Camera.h"
namespace LateralEngine {
	namespace Rendering {
		namespace Opengl {
			class OpenglRenderer {
			public:
				void Clear();
				void Swap(LateralEngine::Window* window);
				void DrawChunk(LateralEngine::Rendering::Chunk* chunk, LateralEngine::Rendering::Camera* camera, LateralEngine::Rendering::Shader* shader);
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
