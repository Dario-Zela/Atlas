#pragma once

namespace Atlas
{
	//Define the needed classes that will be used later
	class RenderQueuePass;
	class Sink;
	class Source;
	class Pass;

	class RenderTarget;
	class DepthStencilBuffer;

	class ThreadPool;
	class DeferredRenderContext;

	//The render graph is the structure that will allow for more
	//Efficient drawings of complex scenes. This will also allow you to
	//Make effects, such as having a blurred screen, ect.
	//It has to be inherited to be used
	class RenderGraph
	{
	public:
		//Initialises the base variables
		RenderGraph();

		//Executes the render graph using parallel processing
		void Execute();
		//Executes the render graph sequentially
		void ExecuteImmediate();

		//Resets the render graph
		void Reset();
		
		//Gets the render queue with the appropriate name
		RenderQueuePass& GetRenderQueue(const std::string& name);

	protected:
		//Sets a target to a global sink
		void SetGlobalSinkTarget(const std::string& sinkName, const std::string& target);
		//Adds a global sink. These must be linked to by the last passes
		void AddGlobalSink(std::unique_ptr<Sink> sink);
		//Adds a global source. These must be linked to the first passes
		void AddGlobalSource(std::unique_ptr<Source> source);
		//Finalises the render graph. This needs to be done before executing the render graph
		void Finalise();
		//Adds a pass to the render graph
		void AddPass(std::unique_ptr<Pass> pass);

		//Gets a pass
		std::unique_ptr<Pass>& GetPass(const std::string& name);

		//The render target
		std::shared_ptr<RenderTarget> m_BackBuffer;
		//The depth buffer
		std::shared_ptr<DepthStencilBuffer> m_DefaultDepth;

	private:
		//Links the sinks of a pass and assigns it a level
		int LinkPassSinks(Pass& pass);
		//Links the global sinks
		void LinkGlobalSinks();

		//The passes of the render graph
		std::vector<std::unique_ptr<Pass>> m_Passes;
		//The global sinks of the render graph
		std::vector<std::unique_ptr<Sink>> m_GlobalSinks;
		//The global sources of the render graph
		std::vector<std::unique_ptr<Source>> m_GlobalSources;
		//The pull of threads to be used
		//A shared pointer is being used to wrap the pointer and
		//as a unique pointer is not usable
		std::shared_ptr<ThreadPool> m_ThreadPool;

		//If the render graph is finalised
		bool m_Finalised;
	};
}