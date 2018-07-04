#include "Window.h"
#include "../Utilities/Input.h"
#include "../Utilities/Log.h"
#include "../glad/glad.h"
#include "../Renderer/OpenglRenderer.h"

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

void Window::LockCursor()
{
	glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	ResetCursor();
}

void Window::UnlockCursor()
{
	glfwSetInputMode(GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::ResetCursor()
{
	glfwSetCursorPos(GetWindow(), GlobalVariables::Window::width / 2, GlobalVariables::Window::height / 2);
}

void Window::Update() 
{
	glfwPollEvents();
}

void Window::PopulateConfig()
{
	GlobalVariables::Window::width = 1280;
	GlobalVariables::Window::height = 720;
	GlobalVariables::Application::ApplicationName = "Lateral -- Test";
}

void ResizeCallback(GLFWwindow * window, int width, int height)
{
	GlobalVariables::Window::width = width;
	GlobalVariables::Window::height = height;

	LateralEngine::Rendering::Opengl::OpenglRenderer::GetInstance()->UpdateViewPort();
}

void Window::CreateWindow()
{
	m_Window = glfwCreateWindow(GlobalVariables::Window::width, GlobalVariables::Window::height, GlobalVariables::Application::ApplicationName, nullptr, nullptr);
	glfwSetCursorPosCallback(m_Window, &Input::UpdateMousePosition);
	glfwSetMouseButtonCallback(m_Window, &Input::UpdateMouseButton);
	glfwSetWindowSizeCallback(m_Window, &ResizeCallback);
	glfwSetKeyCallback(m_Window, &Input::UpdateKey);
	glfwSetWindowAttrib(m_Window, GLFW_AUTO_ICONIFY, true);
	glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, true);

	glfwMakeContextCurrent(GetWindow());
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	Log("Window creation successful!")
}

Window::~Window() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

}