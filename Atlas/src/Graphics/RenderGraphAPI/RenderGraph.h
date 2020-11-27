#pragma once

namespace Atlas
{
	class RenderQueuePass;
	class Sink;
	class Source;
	class Pass;

	class RenderTarget;
	class DepthStencilBuffer;

	class RenderGraph
	{
	public:
		RenderGraph();

		void Execute();
		void Reset();
		
		RenderQueuePass& GetRenderQueue(std::string name);
	protected:
		void SetGlobalSinkTarget(std::string sinkName, std::string target);
		void AddGlobalSink(std::unique_ptr<Sink> sink);
		void AddGlobalSource(std::unique_ptr<Source> source);
		void Finalise();
		void AddPass(std::unique_ptr<Pass> pass);

	private:
		void LinkPassSinks(Pass& pass);
		void LinkGlobalSinks();

		std::vector<std::unique_ptr<Pass>> m_Passes;
		std::vector<std::unique_ptr<Sink>> m_GlobalSinks;
		std::vector<std::unique_ptr<Source>> m_GlobalSources;
		std::shared_ptr<RenderTarget> m_BackBuffer;
		std::shared_ptr<DepthStencilBuffer> m_DefaultDepth;
		bool m_Finalised;
	};
}