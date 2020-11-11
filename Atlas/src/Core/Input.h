#pragma once
#include "pch.h"
#include "Window/Window.h"

namespace Atlas
{
	//Input Poller struct
	//A set of static functions to get information
	//About the application without having to wait for an event
	struct Input
	{
		//This gets the size of the window
		//If it fails it will return -1,-1
		static std::tuple<int,int> GetWindowSize()
		{
			Window* window = Window::s_Instance;
			RECT screenSize = RECT();
			if (GetClientRect(window->GetWindowHandle(), &screenSize))
			{
				uint width = screenSize.right - screenSize.left;
				uint height = screenSize.bottom - screenSize.top;
				return std::tuple<int, int>(width, height);
			}
			else
				return std::tuple<int, int>(-1,-1);
		}

		//This gets the current cursor position
		//It will return -1,-1 if it fails
		static std::tuple<int, int> GetCursorPosition()
		{
			Window* window = Window::s_Instance;
			POINT cursorPos = POINT();
			if(GetCursorPos(&cursorPos) && ScreenToClient(window->GetWindowHandle(), &cursorPos))
				return std::tuple<int, int>(cursorPos.x, cursorPos.y);
			else
				return std::tuple<int, int>(-1, -1);
		}

		//This returns if a mouse button has been pressed
		//As it uses the same code as the key pressed
		//It redirects to it
		static bool IsMouseButtonPressed(int buttonCode)
		{
			return IsKeyPressed(buttonCode);
		}

		//This checks if a key is pressed
		static bool IsKeyPressed(int keyCode)
		{
			Window* window = Window::s_Instance;
			return GetAsyncKeyState(keyCode) < 0;
		}

		//This returns the horizontal scrool
		static float GetHScrool()
		{
			return m_HScroll;
		}

		//This returns the vertical scrool
		static float GetVScrool()
		{
			return m_VScroll;
		}

		//A function that sets the scroll values
		static void SetScroll(float hScroll, float vScroll)
		{
			m_HScroll = hScroll;
			m_VScroll = vScroll;
		}

	private:
		static float m_HScroll;
		static float m_VScroll;
	};
}