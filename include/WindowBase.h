#ifndef WINDOWBASE_H
#define WINDOWBASE_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

class WindowBase
{
public:
	WindowBase();
	virtual ~WindowBase();

	void Run();

private:
	virtual void Init() = 0;
	virtual void Update() = 0;

protected:
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

private:
	// Class state
	GLFWwindow* window = {};
};

#endif