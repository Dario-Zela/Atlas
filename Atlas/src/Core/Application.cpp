#include "pch.h"
#include "Application.h"
#include "Core/TimeStep.h"
#include "Core/Core.h"
#include "Core/Exception.h"

#include "Input.h"
#include "Test.h"
#include "Graphics/D3DWrappers/Mesh.h"
#include "Core/Camera.h"

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
		AT_CORE_INFO("Window Successfully initialised");

		//Initialise the time
		m_LastFrameTime = std::chrono::system_clock::now();

		//If it is in debug mode
		//Initialise the info manager
		#ifdef AT_DEBUG
			m_InfoManager.Init();
		#endif  
	}

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

			Model model(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\sponza\glTF\Sponza.gltf)");
			ModelDrawSettings settings;
			settings.pixelShaderPath = "TestPixel.cso";
			settings.vertexShaderPath = "TestVertex.cso";
			settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 100000);
			settings.proprietiesFlags = (uint)(MeshProprietiesFlags::TEXTURE_COORDINATES | MeshProprietiesFlags::COLOR_DIFFUSE | MeshProprietiesFlags::SHININESS);
			settings.textureFlags = (uint)MeshTextureFlags::DIFFUSE;
			settings.addAnisotropicFiltering = true;
			settings.addMipMapping = true;
			settings.maxAnisotropy = 7;
			Graphics::BindDefaultViewPort();

			float* objRot = new float[3]();
			GUI gui;
			gui.Init();
			gui.AddSliderFloat3("Rot", objRot, -DirectX::XM_2PI, DirectX::XM_2PI, 0.1f);

			Camera camera(1000, -1000, 1);

			//Flushes the BindableLib to remove unused Bindables
			float timeUntilFlush = 10;

			//The main loop
			while (m_Window.isRunning())
			{
				m_Window.Broadcast();			//All events are broadcast at the beginning
				camera.Update();

				//With the chrono libriary, find the time taken to complete the loop
				TimeStep timeStep;
				{
					auto now = std::chrono::system_clock::now();
					timeStep = std::chrono::duration<float>(now - m_LastFrameTime).count();
					m_LastFrameTime = now;			//Save the current time
					timeUntilFlush -= timeStep;
					if (timeUntilFlush < 0)
					{
						BindableLib::Flush();
						timeUntilFlush = 10;
					}
				}
				
				m_Gfx.ClearScreen(0, 0, 1);
				DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * camera.GetTransform();
				model.Draw(settings, trans);

				m_Gfx.EndFrame(1);

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

				//Reset the value of the scrooling
				Input::SetScroll(0, 0);

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