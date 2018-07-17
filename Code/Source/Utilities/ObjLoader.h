#pragma once
#include <string>
#include <vector>

namespace LateralEngine {
		class Mesh;
}

class ObjLoader {
public:
	static void LoadMesh(std::string path, LateralEngine::Mesh* mesh);
};