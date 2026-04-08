#pragma once

#include <d3d11.h>

namespace Editor
{
	class ImGuiManager
	{
	public:
		ImGuiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
		~ImGuiManager();

		void BeginFrame();
		void EndFrame();

		void DrawLayout();
	};
}