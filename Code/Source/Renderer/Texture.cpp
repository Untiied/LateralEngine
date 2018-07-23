#include "Texture.h"
#include "../glad/glad.h"
#include "../Utilities/soil/SOIL.h"

using namespace LateralEngine::Rendering;

Texture::Texture(std::string inpath) {
	LoadTexture(inpath);
}


void Texture::LoadTexture(std::string inpath) {
	path = inpath;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, tex);
}
