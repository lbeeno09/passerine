#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

namespace Engine
{
	class Win32Window
	{
	public:
		Win32Window(int width, int height, const std::wstring& title);
		~Win32Window();

		bool ProcessMessages();

		void CaptureMouse();
		void ReleaseMouse();
		bool IsMouseCaptured() const { return m_mouseCaptured; }

		HWND GetHWND() const { return m_hwnd; }
		UINT GetWidth() const { return m_width; }
		UINT GetHeight() const { return m_height; }
		bool WasResized() const { return m_resizeRequested; }
		void ResetResizeFlag() { m_resizeRequested = false; }

		void SetMousePosition(int x, int y);

	private:
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_hwnd = nullptr;
		HINSTANCE m_hInst = nullptr;
		std::wstring m_className;

		UINT m_width = 0;
		UINT m_height = 0;
		bool m_resizeRequested = false;
		bool m_mouseCaptured = false;
	};
}