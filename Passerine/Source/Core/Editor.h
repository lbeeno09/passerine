#pragma once

#include "Core/Timer.h"
#include "Graphics/GraphicsTypes.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Grid.h"
#include "Renderer/DX11Renderer.h"
#include "UI/ImGuiManager.h"
#include "Window/Win32Window.h"

#include <memory>
#include <string>

class Editor
{
public:
	Editor(int width, int height, const std::wstring& title);
	~Editor();

	void Run();

private:
	void Update(float dt);
	void Render();

	std::unique_ptr<Win32Window> m_window;
	std::unique_ptr<DX11Renderer> m_renderer;
	std::unique_ptr<ImGuiManager> m_imgui;
	std::unique_ptr<Grid> m_grid;
	std::unique_ptr<Mesh> m_cube;

	Camera m_camera;

	Timer m_timer;
	float m_aspectRatio;

	float m_cubeRotation = 0;
	bool m_viewportHovered = false;
	bool m_showDemo = false;
};