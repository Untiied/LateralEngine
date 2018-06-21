#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace LateralEngine{
	void ResizeCallback(GLFWwindow* window, int width, int height);
	class Window {
	public:
		Window();
		~Window();

		bool ShouldClose();

		void Close();

		void Hide();

		void Show();

		void Update();

		GLFWwindow* GetWindow() const { return m_Window; };
	private:
		GLFWwindow* m_Window;
		void PopulateConfig();
		void CreateWindow();
	};
}



