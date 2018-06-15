#include "Input.h"

bool Input::keys[500] = {};
bool Input::mkeys[20] = {};
int Input::oldKeyState[500] = {};
int Input::oldMouseState[20] = {};
GLFWwindow* Input::window = 0;
glm::vec2 Input::MousePosition(0.0f);
