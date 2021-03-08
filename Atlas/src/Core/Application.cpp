#include "pch.h"
#include "Application.h"
#include "Core/TimeStep.h"
#include "Core/Core.h"
#include "Core/Exception.h"
#include "Graphics/BindableLib.h"
#include "Core/Input.h"

namespace Atlas
{
	Application* Application::s_Instance = nullptr;

	void Application::ReorganiseLayers()
	{
		//For each layer storages, attempt to add or remove the layer
		for (auto layer : m_LayersToPop)
		{
			layer->OnDetach();
			AT_CORE_ATTEMPT(InnerPopLayer(layer));
		}
		m_LayersToPop = std::vector<Layer*>();			//Then clear the vector

		for (auto layer : m_LayersToPush)
		{
			layer->OnAttach();
			AT_CORE_ATTEMPT(InnerPushLayer(layer));
		}
		m_LayersToPush = std::vector<Layer*>();

		for (auto layer : m_OverlaysToPop)
		{
			layer->OnDetach();
			AT_CORE_ATTEMPT(InnerPopOverlay(layer));
		}
		m_OverlaysToPop = std::vector<Layer*>();

		for (auto layer : m_OverlaysToPush)
		{
			layer->OnAttach();
			AT_CORE_ATTEMPT(InnerPushOverlay(layer));
		}
		m_OverlaysToPush = std::vector<Layer*>();
	}

	Application::Application(std::string title, uint width, uint height)
		: m_Minimised(false), m_EventManager(), m_Window(&m_EventManager)
	{
		#ifndef AT_DEBUG
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);
		#endif 


		AT_CORE_INFO("Initialising the window")
		//Initialising the window and the instance of the application
		m_Window.Init(title, width, height);
		Application::s_Instance = this;
		AT_CORE_INFO("Window successfully initialised");

		//Sets the time of the first frame to now
		//This avoids having a nonsensical first frame time
		m_LastFrameTime = std::chrono::system_clock::now();

		//If it is in debug mode
		//Initialise the info manager
		#ifdef AT_DEBUG
		AT_CORE_INFO("Initialising the info manager")
			m_InfoManager.Init();
		AT_CORE_INFO("Info Manager successfully initialised")
		#endif  

		//Initialises the d3d11 graphics and constructs the swap chain
		AT_CORE_INFO("Initialising the graphics")
			m_Gfx.Init(m_Window.GetWindowHandle());
		AT_CORE_INFO("Graphics successfully initialised");
	}

	void Application::Run()
	{
		//This checks if an exception has occoured
		try
		{
			//A timer that flushes the BindableLib to remove unused Bindables
			float timeUntilFlush = 10;

			//The main loop
			while (m_Window.isRunning())
			{
				m_Window.Broadcast();			//All events are broadcast at the beginning
				
				//With the chrono libriary, find the time taken to complete the loop
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
				m_Minimised = m_Window.IsMinimised();

				//If the window is not minimised, then update the layer
				if (!m_Minimised)
					//As it is a stack it is run from back to front
					for (auto layer = m_LayerStack.rbegin(); layer != m_LayerStack.rend(); layer++)
					{
							if (m_Window.isRunning()) //Keep on updating it while the window is alive
								(*layer)->OnUpdate(timeStep);
							else
								goto ForcedExit;
					}

				//It there have been any changes in the layers, reorganise them
				if (m_LayersToPush.size() + m_LayersToPop.size() + m_OverlaysToPop.size() + m_OverlaysToPush.size() > 0) ReorganiseLayers();

				//Reset the value of the scrooling
				Input::SetScroll(0, 0);

				//Then dispatch the events
				m_EventManager.PropagateEvents(&m_LayerStack);
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
			AT_CORE_CRITICAL("\n[Exception Type]: {0}\n{1}", "Standard Exeption", e.what());
			__debugbreak();
		}
		catch (...)
		{
			AT_CORE_CRITICAL("\n[Exception Type]: Unknown Exeption\n[Description]: Details Unavailasble");
			__debugbreak();
		}
	}
}