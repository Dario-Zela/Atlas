#pragma once
#include <chrono>

#include "Window/Window.h"
#include "Layer/LayerHolder.h"
#include "Layer/Layer.h"

namespace Atlas
{
	class Application
	{
	public:
		//Initialises the application
		Application(std::string title, uint width, uint height);
		~Application();

		//Allows the user to close the application internally
		inline void Quit() { m_Window.Release(); }

		//Wrappers over the layer/overlay addition and removal
		//Used in the CLI/C++ as it allows them to be added without
		//Causing issues
		inline void PopLayerWrapper(Layer* layer) { m_LayersToPop.push_back(layer); }
		inline void PushLayerWrapper(Layer* layer) { m_LayersToPush.push_back(layer); }

		inline void PopOverlayWrapper(Layer* layer) { m_OverlaysToPop.push_back(layer); }
		inline void PushOverlayWrapper(Layer* layer) { m_OverlaysToPush.push_back(layer); }

		//The rendering loop
		void Run();
	
		//The functions that allow the implementation
		//Of the application to add the initial layers
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		//Adds events to the queue manually
		void AddEventToQueue(Event* e) { m_Window.AddEvent(e); }

	private:
		//A buffer for layer and overlays that need to be
		//Added and removed. This is done so that there aren't
		//Ghost layers being used when one is deleted
		std::vector<Layer*> m_LayersToPop;
		std::vector<Layer*> m_LayersToPush;

		std::vector<Layer*> m_OverlaysToPop;
		std::vector<Layer*> m_OverlaysToPush;
		
		//The function removes and adds the layers
		//In the vectors
		void ReorganiseLayers();

		//Used to reduce resource usage when minimised
		bool m_Minimised;
		
		//The event manager
		EventManager m_EventManager;

		//Reference to the window and itself
		Window m_Window;
		static Application* s_Instance;

		//The stack of all the layers
		LayerStack m_LayerStack;

		//The time of the previous layer
		std::chrono::time_point<std::chrono::system_clock> m_LastFrameTime;
	};
}