#include "Engine/Core/Input.h"

namespace Engine
{
	bool Input::m_keys[256] = { false };
	bool Input::m_keysLast[256] = { false };
	int Input::m_mouseX = 0;
	int Input::m_mouseY = 0;
	float Input::m_mouseDeltaX = 0.0f;
	float Input::m_mouseDeltaY = 0.0f;

	void Input::Initialize()
	{
		memset(m_keys, 0, sizeof(m_keys));
		memset(m_keysLast, 0, sizeof(m_keysLast));
	}

	bool Input::IsKeyDown(unsigned int keycode)
	{
		if(keycode >= 256)
		{
			return false;
		}

		return m_keys[keycode];
	}

	bool Input::IsKeyPressed(unsigned int keycode)
	{
		if(keycode >= 256)
		{
			return false;
		}

		return m_keys[keycode] && !m_keysLast[keycode];
	}

	void Input::GetMousePosition(int& x, int& y)
	{
		x = m_mouseX;
		y = m_mouseY;
	}

	void Input::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if(wParam < 256)
				{
					m_keys[wParam] = true;
				}
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				if(wParam < 256)
				{
					m_keys[wParam] = false;
				}
				break;
			case WM_MOUSEMOVE:
				int newX = LOWORD(lParam);
				int newY = HIWORD(lParam);
				m_mouseDeltaX = (float)(newX - m_mouseX);
				m_mouseDeltaY = (float)(newY - m_mouseY);

				m_mouseX = newX;
				m_mouseY = newY;
				break;
		}
	}

	void Input::EndFrame()
	{
		memcpy(m_keysLast, m_keys, sizeof(m_keys));

		m_mouseDeltaX = m_mouseDeltaY = 0.0f;
	}
}