#include "pch.h"
#include "Pass.h"

#include "Graphics/Bindable.h"
#include "Graphics/D3DWrappers/DepthStencilBuffer.h"
#include "Graphics/D3DWrappers/RenderTarget.h"

namespace Atlas
{
	Pass::Pass(std::string name)
		:m_Name(std::move(name)), m_Level(-1)
	{
		//Creates an placeholder render target and stencil buffer
		m_RenderTarget = RenderTarget::CreateEmpty();
		m_DepthBuffer = DepthStencilBuffer::CreateEmpty();
	}

	Source& Pass::GetSource(std::string& registeredName)
	{
		//Go through all of the sources
		for (auto& source : m_Sources)
		{
			//If you find the right source, return it
			if (source->GetRegisteredName() == registeredName)
			{
				return *source;
			}
		}
		//Else, throw an exception
		AT_CORE_ASSERT_WARG(false, "The requested source {0} does not appear in pass {1}", registeredName, m_Name)
	}

	Sink& Pass::GetSink(std::string& registeredName)
	{
		//Go through all of the sinks
		for (auto& sink : m_Sinks)
		{
			//If you find the right sink, return it
			if (sink->GetRegisteredName() == registeredName)
			{
				return *sink;
			}
		}
		//Else, throw an exception
		AT_CORE_ASSERT_WARG(false, "The requested sink {0} does not appear in pass {1}", registeredName, m_Name)
	}

	void Pass::AddBindable(std::shared_ptr<Bindable> bindable)
	{
		//Add the bindable to the vector
		m_Bindables.push_back(std::move(bindable));
	}

	void Pass::BindAll(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//If the render target isn't null
		if (m_RenderTarget)
		{
			//Bind it with the render target
			m_RenderTarget->Bind(context, m_DepthBuffer->GetDepthStencilBuffer().Get());
		}
		else
		{
			//Else, bind the depth buffer
			m_DepthBuffer->Bind(context);
		}

		//Then bind all of the bindables
		for (auto& bindable : m_Bindables)
		{
			bindable->Bind(context);
		}
	}

	void Pass::BindAll()
	{
		//If the render target isn't null
		if (m_RenderTarget)
		{
			//Bind it with the render target
			m_RenderTarget->ImmidiateBind(m_DepthBuffer->GetDepthStencilBuffer().Get());
		}
		else
		{
			//Else, bind the depth buffer
			m_DepthBuffer->ImmidiateBind();
		}

		//Then bind all of the bindables
		for (auto& bindable : m_Bindables)
		{
			bindable->ImmidiateBind();
		}
	}

	void Pass::SetSinkLink(std::string registeredName, std::string target)
	{
		//Get the sink
		auto& sink = GetSink(registeredName);
		//Set it's target
		sink.SetTarget(target);
	}

	void Pass::Finalise()
	{
		//Check that all of the sinks are linked
		for (auto& sink : m_Sinks)
		{
			sink->ValidateLinks();
		}
		//Check that all of the sources are linked
		for (auto& source : m_Sources)
		{
			source->ValidateLinks();
		}

		//Make sure that there is either a render target or a depth buffer
		AT_CORE_ASSERT_WARG(m_RenderTarget->IsValid() || m_DepthBuffer->IsValid(), "The pass {0} needs either a render target or a depth buffer", m_Name)
		//Make sure the pass was inserted correctly inside the render graph
		AT_CORE_ASSERT_WARG(m_Level > -1, "The pass {0} has not been accurately placed into the graph, this will only occour if there are no sinks or sources to link", m_Name)
	}

	void Pass::RegisterSink(std::unique_ptr<Sink> sink)
	{
		//Make sure that the sink is not duplicate
		for (auto& ownedSink : m_Sinks)
		{
			AT_CORE_ASSERT_WARG(!(ownedSink->GetRegisteredName() == sink->GetRegisteredName()), "The sinks {0} already exists in pass {1}", sink->GetRegisteredName(), m_Name)
		}

		//Add the sink into the pass
		m_Sinks.push_back(std::move(sink));
	}

	void Pass::RegisterSource(std::unique_ptr<Source> source)
	{
		//Make sure that the source is not duplicate
		for (auto& ownedSource : m_Sources)
		{
			AT_CORE_ASSERT_WARG(!(ownedSource->GetRegisteredName() == source->GetRegisteredName()), "The source {0} already exists in pass {1}", source->GetRegisteredName(), m_Name)
		}

		//Add the source into the pass
		m_Sources.push_back(std::move(source));
	}
}