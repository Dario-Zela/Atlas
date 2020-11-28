#include "pch.h"
#include "RenderGraph.h"
#include <thread>

#include "Graphics/Graphics.h"
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/RenderGraphAPI/RenderQueuePass.h"
#include "Graphics/RenderGraphAPI/Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"

namespace Atlas
{
	RenderGraph::RenderGraph()
		: m_BackBuffer(Graphics::GetRenderTarget()), m_DefaultDepth(DepthStencilBuffer::Create())
	{
		AddGlobalSource(Source::Create("backBuffer", m_BackBuffer));
		AddGlobalSource(Source::Create("defaultDepth", m_DefaultDepth));
		AddGlobalSink(Sink::Create("backBuffer", m_BackBuffer));
	}

	void RenderGraph::Execute()
	{
		AT_CORE_ASSERT(m_Finalised, "The Render Graph has not been finalised")

		for (int i = 0; i < m_Passes.size(); i++)
		{
			m_ThreadPool.AddWork(std::bind(&Pass::Execute, m_Passes[i].get(), m_DeferedContexts[i].GetContext()));
		}

		m_ThreadPool.Sync();

		for (int i = 0; i < m_Passes.size(); i++)
		{
			auto& context = m_DeferedContexts[i];
			wrl::ComPtr<ID3D11CommandList> commandList = nullptr;
			context.GetContext()->FinishCommandList(FALSE, &commandList);
			Graphics::GetContext()->ExecuteCommandList(commandList.Get(), FALSE);
		}
	}

	void RenderGraph::ExecuteImmidiate()
	{
		for (auto& pass : m_Passes)
		{
			pass->ExecuteImmidiate();
		}
	}

	RenderQueuePass& RenderGraph::GetRenderQueue(std::string name)
	{
		try
		{
			for (auto& pass : m_Passes)
			{
				if (pass->GetName() == name)
				{
					return dynamic_cast<RenderQueuePass&>(*pass);
				}
			}
		}
		catch (std::bad_cast&)
		{
			AT_CORE_ASSERT_WARG(false, "The pass of name {0} is not a render queue pass", name)
		}
		AT_CORE_ASSERT_WARG(false, "There was no pass of the name {0} in the render graph", name)
	}

	RenderGraph::~RenderGraph()
	{
		delete[] m_DeferedContexts;
	}

	void RenderGraph::SetGlobalSinkTarget(std::string sinkName, std::string target)
	{
		auto finder = [&sinkName](const std::unique_ptr<Sink>& sink)
		{
			return sink->GetRegisteredName() == sinkName;
		};

		auto i = std::find_if(m_GlobalSinks.begin(), m_GlobalSinks.end(), finder);

		AT_CORE_ASSERT_WARG(!(i == m_GlobalSinks.end()), "There is no global sink with the name {0}", sinkName)

		(*i)->SetTarget(target);
	}

	void RenderGraph::AddGlobalSink(std::unique_ptr<Sink> sink)
	{
		m_GlobalSinks.push_back(std::move(sink));
	}

	void RenderGraph::AddGlobalSource(std::unique_ptr<Source> source)
	{
		m_GlobalSources.push_back(std::move(source));
	}

	void RenderGraph::Finalise()
	{
		AT_CORE_ASSERT(!m_Finalised, "The Render Graph has not been finalised")
		
		LinkGlobalSinks();


		std::sort(m_Passes.begin(), m_Passes.end(), [](std::unique_ptr<Pass>& a, std::unique_ptr<Pass>& b)
			{
				return a->GetLevel() < b->GetLevel();
			}
		);

		m_ThreadPool.CreatePool(m_Passes.size());

		m_DeferedContexts = new DeferredRenderContext[m_Passes.size()];

		m_Finalised = true;
	}

	void RenderGraph::AddPass(std::unique_ptr<Pass> pass)
	{
		AT_CORE_ASSERT(!m_Finalised, "The Render Graph has not been finalised")

		for (auto& ownedPass : m_Passes)
		{
			AT_CORE_ASSERT_WARG(!(ownedPass->GetName() == pass->GetName()), "The pass name {0} already exists", pass->GetName())
		}

		pass->m_Level = LinkPassSinks(*pass);

		m_Passes.push_back(std::move(pass));
	}

	void RenderGraph::Reset()
	{
		for (auto& pass : m_Passes)
		{
			pass->Reset();
		}
	}

	int RenderGraph::LinkPassSinks(Pass& pass)
	{
		int level = 0;
		for (auto& sink : pass.GetSinks())
		{
			std::string& sourcePassName = sink->GetPassName();

			if (sourcePassName == "$")
			{
				level = level == 0 ? 0 : level;

				for (auto& source : m_GlobalSources)
				{
					if (source->GetRegisteredName() == sink->GetOutputName())
					{
						sink->Bind(*source);
						goto NextSink;
					}
				}
				AT_CORE_ASSERT_WARG(false, "There is no global source named {0} for the sink {1} in pass {2} to link to", sink->GetOutputName(), sink->GetRegisteredName(), pass.GetName())
			}
			else
			{
				for (auto& pass : m_Passes)
				{
					if (pass->GetName() == sourcePassName)
					{
						auto& source = pass->GetSource(sink->GetOutputName());
						sink->Bind(source);

						if (level - 1 < pass->GetLevel())
							level = pass->GetLevel() + 1;

						goto NextSink;
					}
				}
			}

		NextSink:;
		}

		return level;
	}

	void RenderGraph::LinkGlobalSinks()
	{
		for (auto& sink : m_GlobalSinks)
		{
			std::string& inputSourcePassName = sink->GetPassName();

			for (auto& pass : m_Passes)
			{
				if (pass->GetName() == inputSourcePassName)
				{
					auto& source = pass->GetSource(sink->GetOutputName());
					sink->Bind(source);
					break;
				}
			}
		}
	}
}