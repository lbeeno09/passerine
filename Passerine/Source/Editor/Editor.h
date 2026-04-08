#pragma once

#include "Engine/Core/Timer.h"
#include "Engine/Core/GraphicsTypes.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Mesh.h"
#include "Engine/Scene/Grid.h"
#include "Engine/Renderer/DX11Renderer.h"
#include "Editor/ImGuiManager.h"
#include "Engine/Window/Win32Window.h"

#include <memory>
#include <string>

namespace Editor
{
	class Editor
	{
	public:
		Editor(int width, int height, const std::wstring& title);
		~Editor();

		void Run();

	private:
		void Update(float dt);
		void Render();

		void RenderUI();

		void DrawMainMenuBar();
		void DrawViewport();
		void DrawOutliner();
		void DrawDetails();
		void DrawContentDrawer();
		void DrawConsole();

		void SpawnPrimitive(const std::string& type);
		std::string GetNextAvailableName(const std::string& prefix);

		std::unique_ptr<Engine::Win32Window> m_window;
		std::unique_ptr<Engine::DX11Renderer> m_renderer;
		std::unique_ptr<ImGuiManager> m_imgui;
		std::unique_ptr<Engine::Grid> m_grid;
		//std::unique_ptr<Engine::Mesh> m_cube;
		std::vector<std::unique_ptr<Engine::Entity>> m_sceneEntities;
		std::unique_ptr<Engine::Camera> m_camera;

		Engine::Timer m_timer;
		Engine::Entity* m_selectedEntity = nullptr;
		Engine::Entity* m_renamingEntity = nullptr;
		char m_renameBuffer[64] = "";
		float m_aspectRatio;

		bool m_viewportHovered = false;
		bool m_isNavigating = false;
		int m_anchorX = 0;
		int m_anchorY = 0;

		bool m_showDemo = false;

	};
}