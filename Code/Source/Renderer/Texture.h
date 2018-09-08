#pragma once
#include <string>

namespace LateralEngine {
	namespace Rendering {
		class Texture {
		public:
			inline Texture() {};
			Texture(std::string inpath);
			void LoadTexture(std::string inpath);
		public:
			unsigned int tex;
			unsigned char* image;
			std::string type;
			std::string path;
			int width, height;
			void Bind();
		};
	}
}