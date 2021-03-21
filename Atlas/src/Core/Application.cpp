#include "pch.h"
#include "Application.h"
#include "Core/TimeStep.h"
#include "Core/Core.h"
#include "Core/Exception.h"
#include "Graphics/BindableLib.h"
#include "Core/Input.h"

#include "Events/EventManager.h"
#include "Graphics/DxgiInfoManager.h"
#include "Window/Window.h"
#include "Layer/LayerHolder.h"

namespace Atlas
{
	Application* Application::s_Instance = nullptr;

	void Application::ReorganiseLayers()
	{
		//For each layer storages, attempt to add or remove the layer
		for (auto& layer : m_LayersToPush)
		{
			AT_CORE_ATTEMPT(m_LayerStack->PushLayer(layer));
		}
		m_LayersToPush = std::vector<std::shared_ptr<Layer>>();		//Then clear the vector

		for (auto& layer : m_LayersToPop)
		{
			AT_CORE_ATTEMPT(m_LayerStack->PopLayer(layer));
		}
		m_LayersToPop = std::vector<std::shared_ptr<Layer>>();			

		for (auto& layer : m_OverlaysToPush)
		{
			AT_CORE_ATTEMPT(m_LayerStack->PushOverlay(layer));
		}
		m_OverlaysToPush = std::vector<std::shared_ptr<Layer>>();

		for (auto& layer : m_OverlaysToPop)
		{
			AT_CORE_ATTEMPT(m_LayerStack->PopOverlay(layer));
		}
		m_OverlaysToPop = std::vector<std::shared_ptr<Layer>>();
	}

	Application::Application(const std::string& title, uint width, uint height)
		: m_Minimised(false)
	{
		m_InfoManager = std::make_shared<DxgiInfoManager>();
		m_EventManager = std::make_shared<EventManager>();
		m_Window = std::make_shared<Window>(m_EventManager.get());
		m_LayerStack = std::make_shared<LayerStack>();

		#ifndef AT_DEBUG
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);
		#endif 

		AT_CORE_ASSERT(width * height > 0, "The size of the window must be larger then 0x0")

		AT_CORE_ASSERT(!s_Instance, "An application has already been constructed,\nonly one can be operating at any one time and should be\nreleased before new one is generated")

		Application::s_Instance = this;
		
		AT_CORE_INFO("Initialising the window")
		//Initialising the window and the instance of the application
		m_Window->Init(title, width, height);
		AT_CORE_INFO("Window successfully initialised");

		//Sets the time of the first frame to now
		//This avoids having a nonsensical first frame time
		m_LastFrameTime = std::chrono::system_clock::now();

		//If it is in debug mode
		//Initialise the info manager
		#ifdef AT_DEBUG
		AT_CORE_INFO("Initialising the info manager")
			m_InfoManager->Init();
		AT_CORE_INFO("Info Manager successfully initialised")
		#endif  

		//Initialises the d3d11 graphics and constructs the swap chain
		AT_CORE_INFO("Initialising the graphics")
			m_Gfx.Init(m_Window->GetWindowHandle());
		AT_CORE_INFO("Graphics successfully initialised");
	}

	void Application::Quit() 
	{ 
		AT_CORE_ASSERT(m_Window->isRunning(), "The window is already closed");
		m_Window->Release();
	}

	void Application::PopLayer(std::shared_ptr<Layer> layer)
	{ 
		AT_CORE_ASSERT(layer, "The layer for removal is not initialised")
		m_LayersToPop.push_back(layer); 
	}

	void Application::PushLayer(std::shared_ptr<Layer> layer) 
	{
		AT_CORE_ASSERT(layer, "The layer for addition is not initialised")
		m_LayersToPush.push_back(layer); 
	}

	void Application::PopOverlay(std::shared_ptr<Layer> layer) 
	{
		AT_CORE_ASSERT(layer, "The overlay for removal is not initialised")
		m_OverlaysToPop.push_back(layer); 
	}

	void Application::PushOverlay(std::shared_ptr<Layer> layer) 
	{
		AT_CORE_ASSERT(layer, "The overlay for addition is not initialised")
		m_OverlaysToPush.push_back(layer); 
	}

	void Application::Run()
	{
		//This checks if an exception has occurred
		try
		{
			//A timer that flushes the BindableLib to remove unused Bindables
			float timeUntilFlush = 10;

			//The main loop
			while (m_Window->isRunning())
			{
				m_Window->Broadcast();			//All events are broadcast at the beginning
				
				//With the chrono library, find the time taken to complete the loop
				TimeStep timeStep;
				{
					auto now = std::chrono::system_clock::now();
					timeStep = std::chrono::duration<float>(now - m_LastFrameTime).count();
					m_LastFrameTime = now;			//Save the current time

					//Reduce the time and if it has reached 0
					//Flush the bindableLib and reset the timer
					timeUntilFlush -= timeStep;
					if (timeUntilFlush < 0)
					{
						BindableLib::Flush();
						timeUntilFlush = 10;
					}
				}

				//In debug mode the title of the window is changed to the FPS
				#ifdef AT_DEBUG
					SetWindowTitle("FPS: " + std::to_string(1 / timeStep));
				#endif

				//Update the minimised flag
				m_Minimised = m_Window->IsMinimised();

				//If the window is not minimised, then update the layer
				if (!m_Minimised)
					//As it is a stack it is run from back to front
					for (auto layer : *m_LayerStack.get())
					{
							if (m_Window->isRunning()) //Keep on updating it while the window is alive
								AT_CORE_ATTEMPT(layer->OnUpdate(timeStep))
							else
								goto ForcedExit;
					}

				//It there have been any changes in the layers, reorganise them
				if (m_LayersToPush.size() + m_LayersToPop.size() + m_OverlaysToPop.size() + m_OverlaysToPush.size() > 0) ReorganiseLayers();

				//Reset the value of the scrolling
				Input::SetScroll(0, 0);

				//Then dispatch the events
				m_EventManager->PropagateEvents(m_LayerStack.get());
			}
			//Allows a forced exit that makes sure the
			//Window is edited only when it is alive
		ForcedExit:;
		}
		//The catches will output the error to the console and break the debug
		catch (const AtlasException& e)
		{
			AT_CORE_CRITICAL("\n[Exception Type]: {0}", e.what());
			__debugbreak();
		}
		catch (const std::exception& e)
		{
			AT_CORE_CRITICAL("\n[Exception Type]: {0}\n{1}", "Standard Exception", e.what());
			__debugbreak();
		}
		catch (...)
		{
			AT_CORE_CRITICAL("\n[Exception Type]: Unknown Exception\n[Description]: Details Unavailable");
			__debugbreak();
		}
	}

	void Application::SetWindowTitle(const std::string& Title) 
	{ 
		SetWindowTextA(m_Window->GetWindowHandle(), Title.c_str());
	}

	void Application::AddEventToQueue(Event* e) 
	{ 
		AT_CORE_ASSERT(e, "The event that is being added is not initialised")
		m_Window->AddEvent(e);
	}

	Application* Application::GetInstance() 
	{ 
		AT_CORE_ASSERT(s_Instance, "You are trying to get the application before it has \nbeen generated or after it has been released") 
		return s_Instance; 
	}
}
