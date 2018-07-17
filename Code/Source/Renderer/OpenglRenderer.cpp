#include "OpenglRenderer.h"
#include "../glad/glad.h"
#include "../Utilities/Input.h"
using namespace LateralEngine::Rendering::Opengl;
OpenglRenderer* OpenglRenderer::instance = 0;

void OpenglRenderer::Clear()
{
	glClearColor(0.3f, 0.5f, .8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderer::Swap(LateralEngine::Window* window)
{
	glfwSwapBuffers(window->GetWindow());
}

void OpenglRenderer::Draw()
{

}

void OpenglRenderer::UpdateViewPort()
{
	glViewport(0, 0, GlobalVariables::Window::width, GlobalVariables::Window::height);
}