#pragma once
#include "../Window/Window.h"
namespace LateralEngine {
	namespace Rendering {
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
