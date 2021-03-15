#pragma once
#include <Windows.h>

namespace Atlas
{
    class EventManager;
    class Event;

    class Window
    {
    public:
        Window(EventManager* eventManager);

        //Code to initialise a window
        void Init(const std::string& name, uint width, uint height);
        
        //Code to destroy a window
        void Release();

        //Sends the events to the window procedure
        void Broadcast();

        //Adds and event to the event queue
        void AddEvent(Event* e);

        //Checks if the window is minimised
        bool IsMinimised() { return m_Minimised; }

        ~Window() = default;

        //Checks if the window is still running
        bool isRunning() { return m_Active; };

        //Gets the handle of the window
        HWND GetWindowHandle() { return m_Hwnd; }

        //Reference to the Window
        static Window* s_Instance;

        //Is the window minimised
        bool m_Minimised;
    private:
        //Reads errors during initialisation
        //An exception like the one for graphics could not be used as
        //This occurs before a try and except
        std::string TranslateErrorCode(HRESULT hr);

        //Reference to the Window Handle
        HWND m_Hwnd;
        //Is the window Active
        bool m_Active;

        //Reference to the event manager
        EventManager* m_EventManager;
    };
}