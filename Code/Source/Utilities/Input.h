#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace KeyCode {
	enum KeyCode {
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		ArrowUp = 265,
		ArrowDown = 264,
		ArrowLeft = 263,
		ArrowRight = 262,
		Space = 32,
		ESC = 256,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301
	};
}
class Input {
protected:
	static bool keys[500];
	static int oldKeyState[500];
	static int oldMouseState[20];
	static bool mkeys[20];
public:
	static GLFWwindow* window;
	static glm::vec2 MousePosition;


	static inline void UpdateKey(GLFWwindow* window, int keynumber, int scancode, int action, int mods) {
		keys[keynumber] = action != GLFW_RELEASE;
	};

	static inline void UpdateMouseButton(GLFWwindow* window, int button, int action, int mods) {
		mkeys[button] = action;
	};

	static inline void UpdateMousePosition(GLFWwindow* window, double x, double y) {
		MousePosition = glm::vec2(x, y);
	}

	static inline bool GetKey(KeyCode::KeyCode key) {
		bool FuncKey;
		FuncKey = keys[key];
		return FuncKey;
	};

	static inline float GetHorizontal() {
		bool Left, Right;
		Left = keys[KeyCode::A];
		Right = keys[KeyCode::D];
		if (Left && Right) {
			return 0.0;
		}else if (Left) {
			return -1.0;
		}else if (Right) {
			return 1.0;
		}
		return 0.0;
	};

	static inline float GetVertical() {
		bool Up, Down;
		Up = keys[KeyCode::W];
		Down = keys[KeyCode::S];
		if (Up && Down) {
			return 0.0;
		}
		else if (Down) {
			return -1.0;
		}
		else if (Up) {
			return 1.0;
		}
		return 0.0;
	};
	
	static inline bool GetKeyDown(KeyCode::KeyCode key) {
		int newState = glfwGetKey(window, key);

		if (newState == GLFW_PRESS && oldKeyState[key] == GLFW_RELEASE) {
			oldKeyState[key] = newState;
			return true;
		}
		else {
			oldKeyState[key] = newState;
			return false;
		}
	};

	static inline bool GetKeyUp(KeyCode::KeyCode key) {
		int newState = glfwGetKey(window, key);

		if (newState == GLFW_RELEASE && oldKeyState[key] == GLFW_PRESS) {
			oldKeyState[key] = newState;
			return true;
		}
		else {
			oldKeyState[key] = newState;
			return false;
		}
	};

	static inline bool GetMouse(int MouseButton) {
		bool FuncButton;
		FuncButton = mkeys[MouseButton];
		return FuncButton;
	}

	static inline bool GetMouseDown(int MouseButton) {
		int newState = glfwGetMouseButton(window,MouseButton);

		if (newState == GLFW_PRESS && oldMouseState[MouseButton] == GLFW_RELEASE) {
			oldMouseState[MouseButton] = newState;
			return true;
		}
		else {
			oldMouseState[MouseButton] = newState;
			return false;
		}
	}

	static inline bool GetMouseUp(int MouseButton) {
		int newState = glfwGetMouseButton(window, MouseButton);

		if (newState == GLFW_RELEASE && oldMouseState[MouseButton] == GLFW_PRESS) {
			oldMouseState[MouseButton] = newState;
			return true;
		}
		else {
			oldMouseState[MouseButton] = newState;
			return false;
		}
	}
};