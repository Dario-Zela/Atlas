#pragma once
#include "Graphics/RenderGraphAPI/Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	//Define the needed classes that will be used later
	class RenderTarget;
	class DepthStencilBuffer;
	class Bindable;
	class RenderGraph;

	//A pass is a drawing step of a render graph
	//It will get certain buffers from it's sinks, use the bindables
	//Saved into it and the execute a draw call.
	//It will at last present buffers in as sources.
	//For it be used, it must be inherited into a custom class
	class Pass
	{
		friend RenderGraph;
	public:
		//Initialises key variables and stores a debug name
		Pass(std::string name);
		//Executes the draw pass on a deferred context, must be overwritten
		virtual void Execute(wrl::ComPtr<ID3D11DeviceContext> context) = 0;
		//Executes the draw pass on the immediate context, must be overwritten
		virtual void ExecuteImmidiate() = 0;
		//Returns the debug name
		std::string& GetName() { return m_Name; }

		//Adds a bindable to be used during the execution
		void AddBindable(std::shared_ptr<Bindable> bindable);
		//Make sure that the pass is properly linked in the render graph
		void Finalise();
		
		//Resets the pass
		virtual void Reset() { }

		//Gets the level of the pass
		int GetLevel() { return m_Level; }

		//Links a sink to a source
		void SetSinkLink(std::string registeredName, std::string target);

	protected:
		//Gets all of the sinks in a pass
		std::vector<std::unique_ptr<Sink>>& GetSinks() { return m_Sinks; }
		//Gets the source with the appropriate name
		Source& GetSource(std::string& registeredName);
		//Returns a sink with the appropriate name
		Sink& GetSink(std::string& registeredName);

		//Binds all of the bindables to the deferred context
		void BindAll(wrl::ComPtr<ID3D11DeviceContext> context);
		//Binds all of the bindables to the immediate context
		void BindAll();

		//Adds a sink to the pass, must have a unique name
		void RegisterSink(std::unique_ptr<Sink> sink);
		//Adds a source to the pass, must have a unique name
		void RegisterSource(std::unique_ptr<Source> source);

		//The render target
		std::shared_ptr<RenderTarget> m_RenderTarget;
		//The depth buffer
		std::shared_ptr<DepthStencilBuffer> m_DepthBuffer;

	private:
		//The sinks
		std::vector<std::unique_ptr<Sink>> m_Sinks;
		//The sources
		std::vector<std::unique_ptr<Source>> m_Sources;
		//The bindables
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		//The debug name
		std::string m_Name;
		//The pass' level
		int m_Level;
	};
}