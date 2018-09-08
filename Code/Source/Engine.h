#pragma once
#include "glad/glad.h"
#include "Window/Window.h"
#include "Utilities/Input.h"
#include "Utilities/Globals.h"
#include "Utilities/Log.h"
#include "Window/Window.h"
#include "Renderer/Shader.h"
#include "Renderer/OpenglRenderer.h"
#include "Renderer/Camera.h"
#include <stdlib.h>
#include <time.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Utilities/Log.h"
#include "World/GameObject.h"
#include "Renderer/MeshRenderer.h"
#include <memory>
#include <thread>
#include "Renderer/Texture.h"
#include "Renderer/Model.h"
#include "Utilities/AssetManager.h"
#include "World/Transform.h"
#include "Utilities/AssetManager.h"
#include "Renderer/Mesh.h"
#include "Utilities/Physics.h"
class Engine
{
public:
	Engine() {};
	virtual ~Engine() { end(); };

	void start() {
		init();
		run();
	}
	LateralEngine::Window *GameWindow = new LateralEngine::Window();
private:
	void run() {
		while (!GameWindow->ShouldClose()) {
			GameWindow->Update();
			update();
			render();
		}
	}
protected:
	virtual void init() {};
	virtual void update() {};
	virtual void render() {};
	virtual void end() {};
};
