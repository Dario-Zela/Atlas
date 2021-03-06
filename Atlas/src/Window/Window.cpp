#include "pch.h"
#include "Window.h"

#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

#include "Events/EventManager.h"

namespace Atlas
{
    Window* Window::s_Instance = nullptr;

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
            //Adds MouseMoved Events
        case WM_MOUSEMOVE:
            {
                POINT pos = POINT();
                //This makes sure the position can be taken and mapped to the screen
                if (GetCursorPos(&pos) && ScreenToClient(hwnd, &pos))      
                {
                    MouseMovedEvent* e = new MouseMovedEvent((uint)pos.x, (uint)pos.y);
                    Window::s_Instance->AddEvent(e);
                }
            }
        break;
            
            //Adds MouseScrolled Events
        case WM_MOUSEWHEEL:
            {
                MouseScrolledEvent* e = new MouseScrolledEvent(0, GET_WHEEL_DELTA_WPARAM(wparam));
                Window::s_Instance->AddEvent(e);
            }
        break;
        case WM_MOUSEHWHEEL:
            {
                MouseScrolledEvent* e = new MouseScrolledEvent(GET_WHEEL_DELTA_WPARAM(wparam), 0);
                Window::s_Instance->AddEvent(e);
            }
        break;

            //Adds MousePressed and MouseReleased events
        case WM_LBUTTONUP:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_LBUTTON, (uint)wparam, false);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_LBUTTONDBLCLK:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_LBUTTON, (uint)wparam, true);
                Window::s_Instance->AddEvent(e);
            }
        break;
        case WM_LBUTTONDOWN:
            {
                MouseButtonReleasedEvent* e = new MouseButtonReleasedEvent(MK_LBUTTON);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_RBUTTONUP:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_RBUTTON, (uint)wparam, false);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_RBUTTONDBLCLK:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_RBUTTON, (uint)wparam, true);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_RBUTTONDOWN:
            {
                MouseButtonReleasedEvent* e = new MouseButtonReleasedEvent(MK_RBUTTON);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_MBUTTONUP:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_MBUTTON, (uint)wparam, false);
                Window::s_Instance->AddEvent(e);
            }
        break;
        case WM_MBUTTONDBLCLK:
            {
                MouseButtonPressedEvent* e = new MouseButtonPressedEvent(MK_MBUTTON, (uint)wparam, true);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_MBUTTONDOWN:
            {
                MouseButtonReleasedEvent* e = new MouseButtonReleasedEvent(MK_MBUTTON);
                Window::s_Instance->AddEvent(e);
            }
        break;

            //Adds Events for KeyPressed, KeyReleased and KeyTyped events
        case WM_KEYDOWN:
            {
                KeyPressedEvent* e = new KeyPressedEvent((int)wparam, (lparam & (1 << 30)) >> 30);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_KEYUP:
            {
                KeyReleasedEvent* e = new KeyReleasedEvent((int)wparam);
                Window::s_Instance->AddEvent(e); 
            }
        break;
        case WM_CHAR:
            {
                KeyTypedEvent* e = new KeyTypedEvent((int)wparam);
                Window::s_Instance->AddEvent(e);
            }
        break;

            //Adds ApplicationResize events
        case WM_SIZE:
            {
                RECT screenSize = RECT();
                if (GetClientRect(hwnd, &screenSize))
                {
                    uint width = screenSize.right - screenSize.left;
                    uint height = screenSize.bottom - screenSize.top;
                    WindowResizeEvent* e = new WindowResizeEvent(width, height);
                    Window::s_Instance->AddEvent(e);

                    //Changes the minimised flag
                    if (height == 0 || width == 0)
                        Window::s_Instance->m_Minimised = true;
                    else
                        Window::s_Instance->m_Minimised = false;
                }
            }
        break;

            //Adds Events when the widow is destroyed
        case WM_DESTROY:
            {
                WindowClosedEvent* e = new WindowClosedEvent();
                PostQuitMessage(0);
                Window::s_Instance->Release();
                Window::s_Instance->AddEvent(e); 
            }
        break;
            //All unused events are reassigned to the Default procedure
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return NULL;
    }

    Window::Window(EventManager* eventManager)
        : m_Minimised(false), m_EventManager(eventManager) {AT_CORE_ASSERT(eventManager ,"An unexpected inner error has occured and the event manager is empty")}


    void Window::Init(const std::string& name, uint width, uint height)
    {
        AT_CORE_ASSERT(!s_Instance, "A window has already been constructed")

        s_Instance = this;

        AT_CORE_ASSERT(width * height > 0, "The size of the window must be greater then 0x0")

        //Set the style of the window
        WNDCLASSEX wc;
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        wc.hInstance = NULL;
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = L"MyWindowClass";
        wc.lpszMenuName = L"";
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;

        //Register the window and check if it has been accomplished successfully
        AT_CORE_ASSERT_WARG(RegisterClassEx(&wc), "The class could not be registered\n[Description]: ", TranslateErrorCode(GetLastError()));

        RECT windowSize = RECT();
        windowSize.top = 0;
        windowSize.bottom = height;
        windowSize.left = 0;
        windowSize.right = width;

        AT_CORE_ASSERT_WARG(AdjustWindowRectEx(&windowSize, WS_TILEDWINDOW, FALSE, 0), "The window could not be resized\n[Description]: ", TranslateErrorCode(GetLastError()));

        //Create the window and save the handle to it
        m_Hwnd = CreateWindowExA(WS_EX_APPWINDOW, "MyWindowClass", name.c_str(), WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, NULL, NULL, NULL, NULL);

        //Check if it was successful
        AT_CORE_ASSERT_WARG(m_Hwnd, "The window could not be created\n[Description]: ", TranslateErrorCode(GetLastError()));

        //Show and draw the window
        ShowWindow(m_Hwnd, SW_SHOW);
        UpdateWindow(m_Hwnd);

        //Highlight that the window is active
        m_Active = true;
    }

    void Window::Release()
    {
        //Show that the window is inactive
        m_Active = false;

        //Destroy the window
        AT_CORE_ASSERT_WARG(DestroyWindow(m_Hwnd), "The window could not be destroyed\n[Description]: ", TranslateErrorCode(GetLastError()));
    }

    void Window::Broadcast()
    {
        MSG msg;
        //Checks for all messages in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            //And dispatches them to the WinProc
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //Adds and event to the event queue

    void Window::AddEvent(Event* e) 
    {
       AT_CORE_ASSERT(e, "The event that is being added is not initialised")

        m_EventManager->AddEventToQueue(e); 
    }

    std::string Window::TranslateErrorCode(HRESULT hr)
    {
        //The buffer for the message is initialised and then filled
        char* MsgBuf = nullptr;
        DWORD MsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MsgBuf), 0, nullptr);
        //If the length is 0, the function failed
        if (MsgLen == 0)
            return "Unidentified error";
        //Else, the string is saved into a variable and the returned
        std::string errorString = MsgBuf;
        LocalFree(MsgBuf);
        return errorString;
    }
}