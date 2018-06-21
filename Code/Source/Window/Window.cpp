#include "Window.h"
#include "../Renderer/VulkanRenderer.h"
#include "../Utilities/Input.h"
#include "../Utilities/Log.h"

namespace LateralEngine{

Window::Window() 
{
	glfwInit();
	PopulateConfig();
	CreateWindow();
}

bool Window::ShouldClose() 
{
	return glfwWindowShouldClose(m_Window);
}

void Window::Close()
{
	glfwSetWindowShouldClose(m_Window, true);
}

void Window::Hide()
{
	glfwHideWindow(m_Window);
}

void Window::Show()
{
	glfwShowWindow(m_Window);
}

void Window::Update() 
{
	glfwPollEvents();
}

void Window::PopulateConfig()
{
	GlobalVariables::Window::width = 500;
	GlobalVariables::Window::height = 500;
	GlobalVariables::Application::ApplicationName = "PROJECT NAME HERE";
}

void ResizeCallback(GLFWwindow * window, int width, int height)
{
	GlobalVariables::Window::width = width;
	GlobalVariables::Window::height = height;

	VulkanRenderer::GetInstance()->RecreateSwapchain();
}

void Window::CreateWindow()
{
	Log("Attemping to create a window...")
	//No need to use opengl with this engine.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_Window = glfwCreateWindow(GlobalVariables::Window::width, GlobalVariables::Window::height, GlobalVariables::Application::ApplicationName, nullptr, nullptr);
	glfwSetCursorPosCallback(m_Window, &Input::UpdateMousePosition);
	glfwSetMouseButtonCallback(m_Window, &Input::UpdateMouseButton);
	glfwSetWindowSizeCallback(m_Window, &ResizeCallback);
	glfwSetKeyCallback(m_Window, &Input::UpdateKey);
	glfwSetWindowAttrib(m_Window, GLFW_AUTO_ICONIFY, true);
	glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, true);
	Log("GLFW Window creation successful!")
}

Window::~Window() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

}