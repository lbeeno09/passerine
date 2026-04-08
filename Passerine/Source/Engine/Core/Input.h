#pragma once

#include <Windows.h>

namespace Engine
{
	class Input
	{
	public:
		static void Initialize();

		// Keyboard
		static bool IsKeyDown(unsigned int keycode);
		static bool IsKeyPressed(unsigned int keycode);

		// Mouse
		static void GetMousePosition(int& x, int& y);
		static float GetMouseDeltaX() { return m_mouseDeltaX; }
		static float GetMouseDeltaY() { return m_mouseDeltaY; }

		// called by Window::HandleProc
		static void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		static void EndFrame(); // reset delta and states

	private:
		static bool m_keys[256];
		static bool m_keysLast[256];
		static int m_mouseX, m_mouseY;
		static float m_mouseDeltaX, m_mouseDeltaY;
	};
}