#pragma once
#include "glad/glad.h"
#include "Window/Window.h"
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
