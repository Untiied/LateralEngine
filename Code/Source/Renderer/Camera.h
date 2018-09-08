#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace LateralEngine {
	namespace Rendering {
		class Camera
		{
		public:
			glm::mat4 View;
			Camera(glm::vec3 pos, float fov, float aspect, float znear, float zfar);
			Camera();
			glm::mat4 GetViewProjection();

			void Setup(glm::vec3 pos, float fov, float aspect, float znear, float zfar);

			inline glm::mat4 GetPerspective() {
				return m_perspective;
			};

			inline glm::vec3 GetForwardVector() {
				return glm::vec3(m_forward);
			}

			inline glm::vec3 GetRightVector() {
				return glm::normalize(-glm::cross(m_up, m_forward));
			}

			inline void mouseupdate(glm::vec2& MousePosition);

			void Update();
			//Manually call it.
			void freeCamera();

			void setActive(bool active);
		public:
			glm::mat4 m_perspective = glm::mat4(0.0f);
			glm::vec3 m_position = glm::vec3(0.0f);
			glm::vec3 m_forward = glm::vec3(0.0f);
			glm::vec3 m_up = glm::vec3(0.0f);
			float speed = 5;
		protected:
			glm::vec2 middle = glm::vec2(0.0f);
			glm::vec2 oldMousePosition = glm::vec2(0.0f);
			bool isActive = false;
			float MouseSensitivity = .2f;
			bool first2 = true;
		};
	}
}
