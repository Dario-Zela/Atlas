#include "pch.h"
#include "Window.h"

namespace Atlas
{
    Window* Window::sInstance = nullptr;

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        
        switch (msg)
        {
        case WM_MOUSEMOVE:
            {
                POINT pos = POINT();
                if (GetCursorPos(&pos) && ScreenToClient(hwnd, &pos))
                    std::cout << "X =" << pos.x << "Y =" << pos.y << std::endl;
            }
            break;

            //Events when the window is created
        case WM_CREATE:
            Window::sInstance->onCreate();
            break;
            
            //Events when the widow is destroyed
        case WM_DESTROY:
            Window::sInstance->onDestroy();
            PostQuitMessage(0);
            break;

            //All unused events are reassigned to the Default procedure
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }

        return NULL;
    }

    bool Window::Init(std::string name, unsigned int width, unsigned int height)
    {
        //This converts the easier to call string format into a const wchar_t array
        //As it is what is used in the WNDCLASSEX creation

        std::wstring wideName = L"";
        for(char c : name)
            wideName += (wchar_t)c;

        //Set the style of the window
        WNDCLASSEX wc;
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_CROSS);
        wc.hIcon = LoadIcon(NULL, IDI_ERROR);
        wc.hIconSm = LoadIcon(NULL, IDI_ERROR);
        wc.hInstance = NULL;
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = L"MyWindowClass";
        wc.lpszMenuName = L"";
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        //Register the window and check if it has been accompished successfully
        if (!RegisterClassEx(&wc))
            return false;

        if (!sInstance)
            sInstance = this;

        RECT windowSize = RECT();
        windowSize.top = 0;
        windowSize.bottom = height;
        windowSize.left = 0;
        windowSize.right = width;

        AdjustWindowRectEx(&windowSize, WS_TILEDWINDOW, FALSE, 0);

        //Create the window and save the handle to it
        mHwnd = CreateWindowEx(WS_EX_APPWINDOW, L"MyWindowClass", wideName.c_str(), WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, NULL, NULL, NULL, NULL);

        //Check if it was succesful
        if (!mHwnd)
            return false;

        //Show and draw the window
        ShowWindow(mHwnd, SW_SHOW);
        UpdateWindow(mHwnd);

        //Highlight that the window is active
        mActive = true;

        return true;
    }

    bool Window::Release()
    {
        //Destroy the window
        if (!DestroyWindow(mHwnd))
            return false;

        return true;
    }

    bool Window::Broadcast()
    {
        MSG msg;
        //Checks for all messages in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            //And dispatches them to the WinProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //It then updates the window
        sInstance->onUpdate();

        //Used to make sure the CPU isn't overloaded
        Sleep(0);

        return true;
    }


    void Window::onCreate()
    {
    }

    void Window::onUpdate()
    {
    }

    void Window::onDestroy()
    {
        mActive = false;
    }
}