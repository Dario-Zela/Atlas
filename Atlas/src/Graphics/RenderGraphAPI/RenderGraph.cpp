#include "pch.h"
#include "RenderGraph.h"
#include <thread>

#include "Graphics/Graphics.h"
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/RenderGraphAPI/RenderQueuePass.h"
#include "Graphics/RenderGraphAPI/Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"

#define TRY(x, string) try{x;}catch(std::exception& e){AT_CORE_CRITICAL(string);}

namespace Atlas
{
	RenderGraph::RenderGraph()
		: m_BackBuffer(Graphics::GetRenderTarget()), m_DefaultDepth(DepthStencilBuffer::Create())
	{
		//Adds a source for the render target and depth stencil, as well as a sink for the render target
		AddGlobalSource(Source::Create("backBuffer", m_BackBuffer));
		AddGlobalSource(Source::Create("defaultDepth", m_DefaultDepth));
		AddGlobalSink(Sink::Create("backBuffer", m_BackBuffer));
	}

	void RenderGraph::Execute()
	{
		//Check that the graph has been finalised
		AT_CORE_ASSERT(m_Finalised, "The Render Graph has not been finalised");

		//Create a command list and an array of false-initialised boolean
		wrl::ComPtr<ID3D11CommandList> commandList = nullptr;
		
		bool* passed = new bool[m_Passes.size()]();
		
		//And then make the lambda function that executes the passes
		auto passExecute = [&commandList, &passed](Pass* pass, wrl::ComPtr<ID3D11DeviceContext> context, int index)
		{
			//Execute the pass on its individual deferred context
			pass->Execute(context);
			//If it is not a pass of the first level, wait until it's your turn
			if(index != 0) while (!passed[index - 1]);
			//Adds the commands list to the original one
			context->FinishCommandList(FALSE, &commandList);
			//State that you are done
			passed[index] = true;
		};

		//Add the passes to the thread pool
		for (int i = 0; i < m_Passes.size(); i++)
		{
			m_ThreadPool.AddWork(passExecute, m_Passes[i].get(), i);
		}

		//Wait for all of them to be done
		m_ThreadPool.Sync();
		
		//Then execute all the commands
		Graphics::GetContext()->ExecuteCommandList(commandList.Get(), FALSE);

		//Free the memory
		delete[] passed;

	}

	void RenderGraph::ExecuteImmidiate()
	{
		//For each pass, execute it
		for (auto& pass : m_Passes)
		{
			pass->ExecuteImmidiate();
		}
	}

	RenderQueuePass& RenderGraph::GetRenderQueue(std::string name)
	{
		try
		{
			//See if there is a pass with the name
			for (auto& pass : m_Passes)
			{
				if (pass->GetName() == name)
				{
					//Return the pass as a render queue pass
					return dynamic_cast<RenderQueuePass&>(*pass);
				}
			}
		}
		catch (std::bad_cast&)
		{
			//If it isn't render queue pass, throw an exception
			AT_CORE_ASSERT_WARG(false, "The pass of name {0} is not a render queue pass", name)
		}
		//If it doesn't exist, throw an exception
		AT_CORE_ASSERT_WARG(false, "There was no pass of the name {0} in the render graph", name)
	}

	void RenderGraph::SetGlobalSinkTarget(std::string sinkName, std::string target)
	{
		//Make a lambda function to find the sink
		auto finder = [&sinkName](const std::unique_ptr<Sink>& sink)
		{
			return sink->GetRegisteredName() == sinkName;
		};

		//Look for it
		auto i = std::find_if(m_GlobalSinks.begin(), m_GlobalSinks.end(), finder);

		//If it doesn't exist, throw an exception
		AT_CORE_ASSERT_WARG(!(i == m_GlobalSinks.end()), "There is no global sink with the name {0}", sinkName)

		//Else, set it's target
		(*i)->SetTarget(target);
	}

	void RenderGraph::AddGlobalSink(std::unique_ptr<Sink> sink)
	{
		//Add the sink
		m_GlobalSinks.push_back(std::move(sink));
	}

	void RenderGraph::AddGlobalSource(std::unique_ptr<Source> source)
	{
		//Add the source
		m_GlobalSources.push_back(std::move(source));
	}

	void RenderGraph::Finalise()
	{
		//Check that the render graph has not been finalised
		AT_CORE_ASSERT(!m_Finalised, "The Render Graph has been finalised")

		//Link the global sinks
		LinkGlobalSinks();

		//Make sure that all of the passes are finalised
		for (auto& pass : m_Passes)
		{
			pass->Finalise();
		}

		//Sort them in ascending order of level
		std::sort(m_Passes.begin(), m_Passes.end(), [](std::unique_ptr<Pass>& a, std::unique_ptr<Pass>& b)
			{
				return a->GetLevel() < b->GetLevel();
			}
		);

		//Create a thread-pool a maximum of 5 large
		m_ThreadPool.CreatePool(5 > m_Passes.size() ? m_Passes.size() : 5);

		//Set the flag to true
		m_Finalised = true;
	}

	void RenderGraph::AddPass(std::unique_ptr<Pass> pass)
	{
		//Check that the render graph has not been finalised
		AT_CORE_ASSERT(!m_Finalised, "The Render Graph has been finalised")

		//Make sure the pass doesn't already exist
		//Make sure the pass doesn't already exist
		for (auto& ownedPass : m_Passes)
		{
			AT_CORE_ASSERT_WARG(!(ownedPass->GetName() == pass->GetName()), "The pass name {0} already exists", pass->GetName())
		}

		//Link the pass and set it's level
		pass->m_Level = LinkPassSinks(*pass);

		//Add the pass
		m_Passes.push_back(std::move(pass));
	}

	std::unique_ptr<Pass>& RenderGraph::GetPass(const std::string& name)
	{
		//Try to find the pass
		for (auto& ownedPass : m_Passes)
		{
			if (ownedPass->GetName() == name)
				return ownedPass;
		}
		AT_CORE_ASSERT_WARG(false, "There is no pass named {0} in the render graph", name)
	}

	void RenderGraph::Reset()
	{
		//Reset all of the passes
		for (auto& pass : m_Passes)
		{
			pass->Reset();
		}
	}

	int RenderGraph::LinkPassSinks(Pass& pass)
	{
		//Get an integer to remember it's level
		int level = -1;
		//For each sink
		for (auto& sink : pass.GetSinks())
		{
			//Get it's target
			std::string& sourcePassName = sink->GetPassName();

			//If the source is global
			if (sourcePassName == "$")
			{
				//Set the level
				level = level > 0 ? level : 0;

				//Add the appropriate source
				for (auto& source : m_GlobalSources)
				{
					if (source->GetRegisteredName() == sink->GetSourceName())
					{
						sink->Bind(*source);
						//Go to the next sink
						goto NextSink;
					}
				}
				//If there is no source with that name, throw an exception
				AT_CORE_ASSERT_WARG(false, "There is no global source named {0} for the sink {1} in pass {2} to link to", sink->GetSourceName(), sink->GetRegisteredName(), pass.GetName())
			}
			else
			{
				//Check through the other passes
				for (auto& pass : m_Passes)
				{
					//Find the appropriate pass
					if (pass->GetName() == sourcePassName)
					{
						//Get the source
						auto& source = pass->GetSource(sink->GetSourceName());
						//Bind it to the sink
						sink->Bind(source);

						//And set the level
						if (level - 1 < pass->GetLevel())
							level = pass->GetLevel() + 1;

						//Then go to the next sink
						goto NextSink;
					}
				}
				//If there is no pass with that name, throw an exception
				AT_CORE_ASSERT_WARG(false, "There is no pass named {0} for the sink {1} in pass {2} to link to", sourcePassName, sink->GetRegisteredName(), pass.GetName())
			}

		NextSink:;
		}

		//Return the final level
		return level;
	}

	void RenderGraph::LinkGlobalSinks()
	{
		//For all of the global sinks
		for (auto& sink : m_GlobalSinks)
		{
			//Get the target
			std::string& inputSourcePassName = sink->GetPassName();

			//Then check if any of the passes have the appropriate source
			for (auto& pass : m_Passes)
			{
				if (pass->GetName() == inputSourcePassName)
				{
					//Get it and bind it
					auto& source = pass->GetSource(sink->GetSourceName());
					sink->Bind(source);
					goto NextSink;
				}
			}

			//If there is no pass with that name, throw an exception
			AT_CORE_ASSERT_WARG(false, "There is no pass named {0} for the global sink {1} to link to", inputSourcePassName, sink->GetRegisteredName())

			NextSink:;
		}
	}
}