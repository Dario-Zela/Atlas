#pragma once
#include <Windows.h>

namespace Atlas
{
    class Window
    {
        HWND mHwnd;
        bool mActive;
    public:

        Window() = default;

        //Code to initiasise a window
        bool Init(std::string name, unsigned int width, unsigned int height);
        
        //Code to destroy a window
        bool Release();

        //Event manager
        bool Broadcast();

        ~Window() = default;

        bool isRunning() { return mActive; };

        void onCreate();
        void onUpdate();
        void onDestroy();

        static Window* sInstance;
    };
}