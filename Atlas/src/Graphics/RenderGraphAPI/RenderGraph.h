#pragma once
#include "Graphics/D3DWrappers/DeferredRenderContext.h"
#include "Core/ThreadPool.h"

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
		void ExecuteImmidiate();
		void Reset();
		
		RenderQueuePass& GetRenderQueue(std::string name);
		~RenderGraph();

	protected:
		void SetGlobalSinkTarget(std::string sinkName, std::string target);
		void AddGlobalSink(std::unique_ptr<Sink> sink);
		void AddGlobalSource(std::unique_ptr<Source> source);
		void Finalise();
		void AddPass(std::unique_ptr<Pass> pass);

	private:
		int LinkPassSinks(Pass& pass);
		void LinkGlobalSinks();

		std::vector<std::unique_ptr<Pass>> m_Passes;
		std::vector<std::unique_ptr<Sink>> m_GlobalSinks;
		std::vector<std::unique_ptr<Source>> m_GlobalSources;
		std::shared_ptr<RenderTarget> m_BackBuffer;
		std::shared_ptr<DepthStencilBuffer> m_DefaultDepth;
		DeferredRenderContext m_Context;
		ThreadPool m_ThreadPool;
		DeferredRenderContext** m_DeferedContexts;
		int m_MaxLevel;

		bool m_Finalised;
	};
}