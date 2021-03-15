#include "pch.h"
#include "Input.h"

namespace Atlas
{
	//Declaring the scroll values
	float Input::m_HScroll = 0;
	float Input::m_VScroll = 0;

	bool Input::IsKeyPressed(int keyCode)
	{
		AT_CORE_ATTEMPT(return GetAsyncKeyState(keyCode) < 0)
	}
}