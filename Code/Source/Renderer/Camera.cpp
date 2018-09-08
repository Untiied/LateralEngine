#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "../Utilities/Globals.h"
#include "../Window/Window.h"
#include "../Utilities/Input.h"

using namespace LateralEngine::Rendering;
Camera::Camera() {

}

Camera::Camera(glm::vec3 pos, float fov, float aspect, float znear, float zfar) {
	m_perspective = glm::perspective(fov, aspect, znear, zfar);
	m_position = pos;
	m_forward = glm::vec3(1.0f, 0.0f, 0.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	middle.x = GlobalVariables::Window::width / 2.0f;
	middle.y = GlobalVariables::Window::height / 2.0f;
}

glm::mat4 Camera::GetViewProjection() {
	View = glm::lookAt(m_position, m_position + m_forward, m_up);
	return View;
}

void Camera::Setup(glm::vec3 pos, float fov, float aspect, float znear, float zfar) {
	m_perspective = glm::perspective(fov, aspect, znear, zfar);
	m_position = pos;
	m_forward = glm::vec3(1.0f, 0.0f, 0.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	middle.x = GlobalVariables::Window::width / 2.0f;
	middle.y = GlobalVariables::Window::height / 2.0f;
}

void Camera::mouseupdate(glm::vec2& MousePosition) {
	float horizMovement = 0.00f;
	float vertMovement = 0.00f;
	if (MousePosition != glm::vec2(0.0f)) {
		horizMovement = (MousePosition.x - middle.x) * MouseSensitivity;
		vertMovement = (MousePosition.y - middle.y) * MouseSensitivity;
		vertMovement = glm::clamp(vertMovement, -89.0f, 89.0f);
		glm::vec3 front = glm::vec3(0,0,0);
		front.x += cos(glm::radians(horizMovement)) * cos(glm::radians(vertMovement));
		front.y += sin(glm::radians(-vertMovement));
		front.z += sin(glm::radians(horizMovement)) * cos(glm::radians(vertMovement));
		m_forward = glm::normalize(front);
	}
}

void Camera::Update() {
	mouseupdate(Input::MousePosition);
}

void Camera::freeCamera()
{
	if (Input::GetKey(KeyCode::W)) {
		m_position += GetForwardVector() * speed * Time::deltaTime;
	}
	if (Input::GetKey(KeyCode::S)) {
		m_position += -GetForwardVector() * speed * Time::deltaTime;
	}
	if (Input::GetKey(KeyCode::A)) {
		m_position += -GetRightVector() * speed * Time::deltaTime;
	}
	if (Input::GetKey(KeyCode::D)) {
		m_position += GetRightVector() * speed * Time::deltaTime;
	}
}

void Camera::setActive(bool active) {
}