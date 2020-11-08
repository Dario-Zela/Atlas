#pragma comment(lib, "comctl32.lib")
#include "pch.h"
#include "GUI.h"
#include <commctrl.h>
#include <windowsx.h>

namespace Atlas
{
	struct SliderData
	{
		UINT code;
		float scale;
	};

	std::unordered_map<HWND, std::tuple<void*, SliderData>> GUI::m_SliderElements = std::unordered_map<HWND, std::tuple<void*, SliderData>>();
	std::unordered_map<HWND, bool*> GUI::m_ButtonElements = std::unordered_map<HWND, bool*>();

	LRESULT CALLBACK GUIProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_HSCROLL:
		{
			if (LOWORD(wparam) == TB_THUMBPOSITION || LOWORD(wparam) == TB_THUMBTRACK)
			{
				auto tot = GUI::m_SliderElements[(HWND)lparam];
				auto com = std::get<1>(tot);
				switch (com.code)
				{
				case 0:
				{
					float* val = (float*)std::get<0>(tot);
					*val = (short)HIWORD(wparam) * com.scale;
				}
				break;
				case 1:
				{
					UINT* val = (UINT*)std::get<0>(tot);
					*val = (short)HIWORD(wparam) * com.scale;
				}
				break;
				case 2:
				{
					int* val = (int*)std::get<0>(tot);
					*val = (short)HIWORD(wparam) * com.scale;
				}
				break;
				}
			}
			else if (LOWORD(wparam) == SB_LINELEFT || LOWORD(wparam) == SB_LINERIGHT ||
				LOWORD(wparam) == TB_PAGEDOWN || LOWORD(wparam) == TB_PAGEUP)
			{
				auto tot = GUI::m_SliderElements[(HWND)lparam];
				auto com = std::get<1>(tot);
				auto pos = SendMessage((HWND)lparam, TBM_GETPOS, 0, 0);
				switch (com.code)
				{
				case 0:
				{
					float* val = (float*)std::get<0>(tot);
					*val = pos * com.scale;
				}
				break;
				case 1:
				{
					UINT* val = (UINT*)std::get<0>(tot);
					*val = pos * com.scale;
				}
				break;
				case 2:
				{
					int* val = (int*)std::get<0>(tot);
					*val = pos * com.scale;
				}
				break;
				}
			}
			break;
		}
		case WM_COMMAND:
		{
			if (HIWORD(wparam) == BN_CLICKED)
			{
				bool* button = GUI::m_ButtonElements[(HWND)lparam];
				*button = !*button;
			}
		}
		break;
		//Adds Events when the widow is destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
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

		RegisterClassEx(&wc);

		RECT windowSize = RECT();
		windowSize.top = 0;
		windowSize.bottom = 400;
		windowSize.left = 0;
		windowSize.right = 500;

		AdjustWindowRectEx(&windowSize, WS_TILEDWINDOW, FALSE, 0);

		//Create the window and save the handle to it
		m_hWnd = CreateWindowExA(WS_EX_APPWINDOW, "MyGUIClass", windowName.c_str(), WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, NULL, NULL, NULL, NULL);

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

		m_TimeAfterTest++;

		if (m_TimeAfterTest == 10)
		{
			for (auto pair : m_SliderElements)
			{
				auto handle = pair.first;
				auto contr = std::get<1>(pair.second);
				switch (contr.code)
				{
				case 0:
				{
					int pos = SendMessage(handle, TBM_GETPOS, 0, 0);
					float* val = (float*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, *val / contr.scale);
				}
				break;

				case 1:
				{
					int pos = SendMessage(handle, TBM_GETPOS, 0, 0);
					UINT* val = (UINT*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, *val / contr.scale);
				}
				break;
				case 2:
				{
					int pos = SendMessage(handle, TBM_GETPOS, 0, 0);
					int* val = (int*)std::get<0>(pair.second);
					if (*val / contr.scale != pos)
						SendMessage(handle, TBM_SETPOS, true, *val / contr.scale);
				}
				break;
				}

				for (auto val : m_ButtonElements)
				{
					bool state = SendMessage(val.first, BM_GETSTATE, 0, 0);
					if (state != *val.second)
					{
						SendMessage(val.first, BM_SETCHECK, (WPARAM)(int)*val.second, 0L);
					}
				}
			}
			m_TimeAfterTest = 0;
		}
	}

	void GUI::AddSliderFloat(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);

		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderFloat2(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);

		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderFloat3(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderFloat4(std::string name, float* val, float min, float max, float scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 0;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
	}
	void GUI::AddSliderUint(std::string name, UINT* val, UINT min, UINT max, UINT scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderUint2(std::string name, UINT* val, UINT min, UINT max, UINT scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderUint3(std::string name, UINT* val, UINT min, UINT max, UINT scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderUint4(std::string name, UINT* val, UINT min, UINT max, UINT scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 1;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = scale;
		m_Widgets++;
		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);
	}
	void GUI::AddSliderInt2(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 110 + 10, 40 + 40 * m_Widgets, 115, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt3(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 + 10, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 80 * 2 + 10 * 2, 40 + 40 * m_Widgets, 80, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);
		m_Widgets++;
	}
	void GUI::AddSliderInt4(std::string name, int* val, int min, int max, int scale)
	{
		HWND hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		HDC hdc = GetDC(m_hWnd);
		RECT rect;
		rect.top = 40 + 40 * m_Widgets;
		rect.left = 20;

		SetBkMode(hdc, TRANSPARENT);
		DrawTextA(hdc, name.c_str(), name.length(), &rect, 0);

		short validMin = min / (float)scale;
		short validMax = max / (float)scale;

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		auto data = SliderData();
		data.code = 2;
		data.scale = scale;

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 + 20, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 1, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 2 + 20 * 2, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 2, data);

		hTrack = CreateWindowExA(0, TRACKBAR_CLASSA, "Trackbar Control",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
			name.length() * 9 + 20 + 50 * 3 + 20 * 3, 40 + 40 * m_Widgets, 70, 30, m_hWnd, NULL, NULL, NULL);

		SendMessage(hTrack, TBM_SETRANGE, true, MAKELPARAM(validMin, validMax));
		SendMessage(hTrack, TBM_SETTICFREQ, std::ceil((validMax - validMin) / 10.0f), 0);

		m_SliderElements[hTrack] = std::tuple<void*, SliderData>(val + 3, data);
		m_Widgets++;
	}

	void GUI::AddCheck(std::string name, bool* val)
	{
		HWND hButton = CreateWindowExA(0, "BUTTON", name.c_str(),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			20, 40 + 40 * m_Widgets, 240, 30, m_hWnd, NULL, NULL, NULL);

		m_ButtonElements[hButton] = val;
		Button_SetCheck(hButton, *val);

		SendMessage(hButton, BM_SETCHECK, (WPARAM)(int)*val, 0L);
		m_Widgets++;
	}
}