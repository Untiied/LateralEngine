#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

namespace LateralEngine {
	namespace Rendering {
		class Shader {
		public:
			Shader(std::string VertexPath = "", std::string FragmentPath = "");
			~Shader();

			void LoadShader(const char* VertexShaderPath, const char* FragmentShaderPath);

			void setBool(const std::string &name, bool value) const;
			void setInt(const std::string &name, int value) const;
			void setFloat(const std::string &name, float value) const;
			void setVec2(const std::string &name, const glm::vec2 &value) const;
			void setVec2(const std::string &name, float x, float y) const;
			void setVec3(const std::string &name, const glm::vec3 &value) const;
			void setVec3(const std::string &name, float x, float y, float z) const;
			void setVec4(const std::string &name, const glm::vec4 &value) const;
			void setVec4(const std::string &name, float x, float y, float z, float w) const;
			void setMat2(const std::string &name, const glm::mat2 &mat) const;
			void setMat3(const std::string &name, const glm::mat3 &mat) const;
			void setMat4(const std::string &name, const glm::mat4 &mat) const;

			void Bind();

			unsigned int GetShaderProgram() {
				return shaderProgram;
			}
		private:
			unsigned int shaderProgram;
			unsigned int vertexShader;
			unsigned int fragmentShader;
		};
	}
}
