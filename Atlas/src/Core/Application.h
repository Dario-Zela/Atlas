#pragma once
#include <chrono>

#include "Layer/Layer.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class EventManager;
	class Window;
	class DxgiInfoManager;
	class LayerStack;

	//A class that holds layers and manages the creation of the window,
	//graphics context and logging information.
	class Application
	{
	public:
		//Initialises the application
		Application(const std::string& title, uint width, uint height);
		~Application() = default;

		//Allows the user to close the application internally
		void Quit();

		//Removes a layer
		void PopLayer(std::shared_ptr<Layer> layer);
		//Adds a layer
		void PushLayer(std::shared_ptr<Layer> layer);

		//Removes an overlay
		void PopOverlay(std::shared_ptr<Layer> layer);
		//Adds an overlay
		void PushOverlay(std::shared_ptr<Layer> layer);

		//Starts rendering loop
		void Run();

		//Allows the client to change the title of the screen dynamically
		void SetWindowTitle(const std::string& Title);

		//Adds events to the queue manually
		void AddEventToQueue(Event* e);

		//Gets the static instance of the window
		static Application* GetInstance();

	private:
		//A buffer for layer and overlays that need to be
		//Added and removed. This is done so that there aren't
		//Ghost layers being used when one is deleted
		std::vector<std::shared_ptr<Layer>> m_LayersToPop;
		std::vector<std::shared_ptr<Layer>> m_LayersToPush;

		std::vector<std::shared_ptr<Layer>> m_OverlaysToPop;
		std::vector<std::shared_ptr<Layer>> m_OverlaysToPush;
		
		//The function removes and adds the layers
		//In the vectors
		void ReorganiseLayers();

		//Used to reduce resource usage when minimised
		bool m_Minimised;
		
		//The event manager
		std::shared_ptr<EventManager> m_EventManager;

		//Reference to the window and itself
		std::shared_ptr<Window> m_Window;
		static Application* s_Instance;

		//The stack of all the layers
		std::shared_ptr<LayerStack> m_LayerStack;

		//The time of the previous layer
		std::chrono::time_point<std::chrono::system_clock> m_LastFrameTime;

		//The graphics object
		Graphics m_Gfx;

		//The Graphics debug object
		std::shared_ptr<DxgiInfoManager> m_InfoManager;
	};
}