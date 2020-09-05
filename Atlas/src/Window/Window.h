#pragma once
#include <Windows.h>
#include "Events/EventManager.h"

namespace Atlas
{
    class Window
    {
    public:
        Window(EventManager* eventManager);

        //Code to initiasise a window
        bool Init(std::string name, uint width, uint height);
        
        //Code to destroy a window
        bool Release();

        //Sends the events to the window procedure
        bool Broadcast();

        //Adds and event to the event queue
        void AddEvent(Event* e) { m_EventManager->AddEventToQueue(e); }

        //Checks if the window is minimised
        bool IsMinimised() { return m_Minimised; }

        ~Window() = default;

        bool isRunning() { return m_Active; };

        //Reference to the Window
        static Window* s_Instance;

        //Is the window minimised
        bool m_Minimised;
    private:
        //Reference to the Window Handle
        HWND m_Hwnd;
        //Is the window Active
        bool m_Active;

        //Reference to the event manager
        EventManager* m_EventManager;
    };
}