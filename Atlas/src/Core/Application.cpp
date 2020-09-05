#include "pch.h"
#include "Application.h"
#include "Core/TimeStep.h"
#include "Core/Core.h"

namespace Atlas
{
	Application* Application::s_Instance = nullptr;

	void Application::ReorganiseLayers()
	{
		//For each layer storages, attempt to add or remove the layer
		for (auto layer : m_LayersToPop)
		{
			AT_CORE_ATTEMPT(PopLayer(layer));
		}
		m_LayersToPop = std::vector<Layer*>();			//Then clear the vector

		for (auto layer : m_LayersToPush)
		{
			AT_CORE_ATTEMPT(PushLayer(layer));
		}
		m_LayersToPush = std::vector<Layer*>();

		for (auto layer : m_OverlaysToPop)
		{
			AT_CORE_ATTEMPT(PopOverlay(layer));
		}
		m_OverlaysToPop = std::vector<Layer*>();

		for (auto layer : m_OverlaysToPush)
		{
			AT_CORE_ATTEMPT(PushOverlay(layer));
		}
		m_OverlaysToPush = std::vector<Layer*>();
	}

	Application::Application(std::string title, uint width, uint height)
		: m_Minimised(false), m_EventManager(), m_Window(&m_EventManager)
	{
		//Initialising the window and the instance of the application
		m_Window.Init(title, width, height);
		Application::s_Instance = this;
	}

	Application::~Application(){}
	
	void Application::Run()
	{
		//The main loop
		while (m_Window.isRunning())
		{
			m_Window.Broadcast();			//All events are broadcast at the beginning

			//With the chrono libriary, find the time taken to complete the loop
			auto now = std::chrono::system_clock::now();
			TimeStep timeStep = std::chrono::duration<float>(now - m_LastFrameTime).count();
			m_LastFrameTime = now;

			//Update the minimised flag
			m_Minimised = m_Window.IsMinimised();

			//If the window is not minimised, then update the layer
			if (!m_Minimised)
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timeStep);

			//Then run dispatch the events
			m_EventManager.PropagateEvents(&m_LayerStack);
		}
	}

	//The LayerStack Wrapper functions
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
	}
}