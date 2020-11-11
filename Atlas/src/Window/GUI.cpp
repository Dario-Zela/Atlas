#pragma comment(lib, "comctl32.lib")
#include "pch.h"
#include "GUI.h"
#include <commctrl.h>
#include <windowsx.h>

namespace Atlas
{
	//A struct that defines a slider
	struct SliderData
	{
		uint code;
		float scale;
	};

	//Declaring the maps
	std::unordered_map<HWND, std::tuple<void*, SliderData>> GUI::m_SliderElements = std::unordered_map<HWND, std::tuple<void*, SliderData>>();
	std::unordered_map<HWND, bool*> GUI::m_ButtonElements = std::unordered_map<HWND, bool*>();

	//The window proc for the gui
	LRESULT CALLBACK GUIProc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_HSCROLL:
		{
			//If the mouse slides the slider
			if (LOWORD(wparam) == TB_THUMBPOSITION || LOWORD(wparam) == TB_THUMBTRACK)
			{
				//Get the data
				auto slider = GUI::m_SliderElements[(HWND)lparam];
				auto data = std::get<1>(slider);
				
				//Switch on the code to get the actual type
				//And update the value
				switch (data.code)
				{
				case 0:
				{
					float* value = (float*)std::get<0>(slider);
					*value = (float)((short)HIWORD(wparam) * data.scale);
				}
				break;
				case 1:
				{
					uint* value = (uint*)std::get<0>(slider);
					*value = (uint)((short)HIWORD(wparam) * data.scale);
				}
				break;
				case 2:
				{
					int* value = (int*)std::get<0>(slider);
					*value = (int)((short)HIWORD(wparam) * data.scale);
				}
				break;
				}
			}
			//If the slider has only changed position
			else if (LOWORD(wparam) == SB_LINELEFT || LOWORD(wparam) == SB_LINERIGHT ||
				LOWORD(wparam) == TB_PAGEDOWN || LOWORD(wparam) == TB_PAGEUP)
			{
				//Get the data and new position
				auto slider = GUI::m_SliderElements[(HWND)lparam];
				auto data = std::get<1>(slider);
				auto position = SendMessage((HWND)lparam, TBM_GETPOS, 0, 0);

				//Switch on the code to get the actual type
				//And update the value
				switch (data.code)
				{
				case 0:
				{
					float* value = (float*)std::get<0>(slider);
					*value = (float)(position * data.scale);
				}
				break;
				case 1:
				{
					uint* value = (uint*)std::get<0>(slider);
					*value = (uint)(position * data.scale);
				}
				break;
				case 2:
				{
					int* value = (int*)std::get<0>(slider);
					*value = (int)(position * data.scale);
				}
				break;
				}
			}
			break;
		}
		case WM_COMMAND:
		{
			//If a button has been cliked
			//Toggle it's value
			if (HIWORD(wparam) == BN_CLICKED)
			{
				bool* button = GUI::m_ButtonElements[(HWND)lparam];
				*button = !*button;
			}
		}
		break;
		//All unused events are reassigned to the Default procedure
		default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		}
		return NULL;
	}

	void GUI::Init(std::string windowName)
	{
		//Create the window descriptor
		WNDCLASSEX wc;
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance = NULL;
		wc.lpfnWndProc = GUIProc;
		wc.lpszClassName = L"MyGUIClass";
		wc.lpszMenuName = L"";
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;

		//Register the class
		RegisterClassEx(&wc);

		RECT windowSize = RECT();
		windowSize.top = 0;
		windowSize.bottom = 400;
		windowSize.left = 0;
		windowSize.right = 500;

		//Adjust the window size
		AdjustWindowRectEx(&windowSize, WS_TILEDWINDOW, FALSE, 0);

		//Create the window and save the handle to it
		m_hWnd = CreateWindowExA(WS_EX_APPWINDOW, "MyGUIClass", windowName.c_str(), WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, NULL, NULL, NULL, NULL);

		//Disable the close button
		EnableMenuItem(GetSystemMenu(m_hWnd, FALSE), SC_CLOSE,
			MF_BYCOMMAND | MF_DISABLED);

		//Show and draw the window
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		//Highlight that the window is active
		m_Alive = true;

		m_TimeAfterTest = 0;
	}

	void GUI::Broadcast()
	{
		//If the window is dead, close the window
		if (m_Alive == false)
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, NULL);
			return;
		}

		MSG msg;
		//Checks for all messages in the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			//And dispatches them to the WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Used to make sure the CPU isn't overloaded
		Sleep(0);

		//This is used to check if the data has chaned values
		//And the slider has not
		m_TimeAfterTest++;

		//Every 10 iteration, the data is checked
		if (m_TimeAfterTest == 10)
		{
			//If there are any differences
			//The slider is updated
			for (auto pair : m_SliderElements)
			{
				auto handle = pair.first;
				auto contr = std::get<1>(pair.second);
				switch (contr.code)
				{
				case 0:
				{
					int pos = (int)SendMessage(handle, TBM_GETPOS, 0, 0);
					float* val = (float*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, (LPARAM)(*val / contr.scale));
				}
				break;

				case 1:
				{
					int pos = (int)SendMessage(handle, TBM_GETPOS, 0, 0);
					uint* val = (uint*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, (LPARAM)(*val / contr.scale));
				}
				break;
				case 2:
				{
					int pos = (int)SendMessage(handle, TBM_GETPOS, 0, 0);
					int* val = (int*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, (LPARAM)(*val / contr.scale));
				}
				break;
				}

				for (auto val : m_ButtonElements)
				{
					bool state = (bool)SendMessage(val.first, BM_GETSTATE, 0, 0);
					if (state != *val.second)
					{
						SendMessage(val.first, BM_SETCHECK, (WPARAM)(int)*val.second, 0L);
					}
				}
			}
			//The time is reset
			m_TimeAfterTest = 0;
		}
	}

	//All slider construction is identical
	//Only AddSliderFloat is commented
	void GUI::AddSliderFloat(std::string name, float* val, float min, float max, float scale)
	{
		//Creates a trackbar control
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		//Adds a tag
		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);

		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		//Sets the tick frequency
		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		//And adds the data to the slider map
		auto data = SliderData();
		data.code = 0;
		data.scale = (float)scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderFloat2(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);

		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)((short)(max / (float)scale));

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderFloat3(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderFloat4(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
	}
	void GUI::AddSliderUint(std::string name, uint* val, uint min, uint max, uint scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = (float)scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderUint2(std::string name, uint* val, uint min, uint max, uint scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderUint3(std::string name, uint* val, uint min, uint max, uint scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderUint4(std::string name, uint* val, uint min, uint max, uint scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = (float)scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderInt2(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt3(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt4(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), (int)name.length(), &rect, 0);

		short validMin = (short)(min / (float)scale);
		short validMax = (short)(max / (float)scale);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = (float)scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			(int)name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, (WPARAM)std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
		m_Widgets++;
	}

	void GUI::AddCheckBox(std::string name, bool* val)
	{
		//A button is created
		HWND hButton = CreateWindowExA(0, "BUTTON", name.c_str(),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		//The value is added to the button elements
		m_ButtonElements[hButton] = val;
		Button_SetCheck(hButton, *val);

		//The value of the button is at last set the the original value
		SendMessage(hButton, BM_SETCHECK, (WPARAM)(int)*val, 0L);
		m_Widgets++;
	}
}