#include "pch.h"
#include "Application.h"
#include "Core/TimeStep.h"
#include "Core/Core.h"
#include "Core/Exception.h"

#include "Input.h"
#include "Test.h"

#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/D3DWrappers/InputLayout.h"
#include "Graphics/D3DWrappers/ViewPort.h"

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
		AT_CORE_INFO("Initialising the window")
		//Initialising the window and the instance of the application
		m_Window.Init(title, width, height);
		Application::s_Instance = this;
		AT_CORE_INFO("Window Successfully initialised")
	}
	
	static auto beg = std::chrono::system_clock::now();

	void Application::Run()
	{
		//This checks if an exception has occoured
		try
		{
			AT_CORE_INFO("Initialising the graphics")
			//The graphics are initialised here to get access to
			//The debug information that it taken from the exception
			m_Gfx.Init(m_Window.GetWindowHandle());
			AT_CORE_INFO("Graphics Successfully initialised");

			std::vector<std::unique_ptr<Box>> boxes;
			{
				std::mt19937 rng(std::random_device{}());
				std::uniform_real_distribution<float> adist(0.0f, DirectX::XM_2PI);
				std::uniform_real_distribution<float> ddist(0.0f, DirectX::XM_2PI);
				std::uniform_real_distribution<float> odist(0.0f, DirectX::XM_PI * 0.3f);
				std::uniform_real_distribution<float> rdist(6.0f, 20);
				for (int i = 0; i < 20; i++)
				{
					boxes.push_back(std::make_unique<Box>(rng, adist, ddist, odist, rdist));
				}
			}

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
				}
				float c = std::sin(std::chrono::duration<float>(beg - m_LastFrameTime).count()) / 2.0f + 0.5f;
				
				m_Gfx.ClearScreen(1, 0, 1);

				for (auto& b : boxes)
				{
					b->Update(c / 25.0f);
					b->Draw();
				}
				m_Gfx.EndFrame(1);
	


				/*
				m_Gfx.ClearScreen(c, c, 0);
				auto var = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(c * DirectX::XM_2PI) * DirectX::XMMatrixTranspose(transform));
				vcb.Update((void*)&var, sizeof(var));
				vcb.Bind();
				box.Update(c / 4.0f);
				box.Draw();
				*/

				#ifdef AT_DEBUG
					SetWindowTitle("FPS: " + std::to_string(1.0f / timeStep));
				#endif

				//Update the minimised flag
				m_Minimised = m_Window.IsMinimised();

				//If the window is not minimised, then update the layer
				if (!m_Minimised)
					for (Layer* layer : m_LayerStack)
					{
						if (m_Window.isRunning()) //Keep on updating it while the window is alive
							layer->OnUpdate(timeStep);
						else
							goto ForcedExit;
					}

				//Then run dispatch the events
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
			AT_CORE_CRITICAL("\n[Exception Type]: Unknown Exeption\n[Description]: Details Unavailable");
			__debugbreak();
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