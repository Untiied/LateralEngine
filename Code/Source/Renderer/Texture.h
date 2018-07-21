#pragma once
#include <string>

namespace LateralEngine {
	namespace Rendering {
		class Texture {
		public:
			Texture(std::string path);
			~Texture();
		public:
			unsigned int tex;
			unsigned char* image;
			int width, height;
			void Bind();
		};
	}
}