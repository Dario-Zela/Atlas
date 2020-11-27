#include "pch.h"
#include "Pass.h"

#include "Graphics/Bindable.h"

#include "Graphics/D3DWrappers/DepthStencilBuffer.h"
#include "Graphics/D3DWrappers/RenderTarget.h"

namespace Atlas
{
	Pass::Pass(std::string name)
		:m_Name(std::move(name)) 
	{
		m_RenderTarget = RenderTarget::Create(1,1);
		m_DepthBuffer = DepthStencilBuffer::Create(1,1);
	}

	Source& Pass::GetSource(std::string& registeredName)
	{
		for (auto& source : m_Sources)
		{
			if (source->GetRegisteredName() == registeredName)
			{
				return *source;
			}
		}
		AT_CORE_ASSERT(false, "The requested source {0} does not appear in pass {1}", registeredName, m_Name)
	}

	Sink& Pass::GetSink(std::string& registeredName)
	{
		for (auto& sink : m_Sinks)
		{
			if (sink->GetRegisteredName() == registeredName)
			{
				return *sink;
			}
		}
		AT_CORE_ASSERT(false, "The requested sink {0} does not appear in pass {1}", registeredName, m_Name)
	}

	void Pass::AddBindable(std::shared_ptr<Bindable> bindable)
	{
		m_Bindables.push_back(std::move(bindable));
	}

	void Pass::BindAll()
	{
		if (m_RenderTarget)
		{
			m_RenderTarget->Bind(m_DepthBuffer->GetDepthStencilBuffer().Get());
		}
		else
		{
			m_DepthBuffer->Bind();
		}

		m_RenderTarget->Debug = true;

		for (auto& bindable : m_Bindables)
		{
			bindable->Bind();
		}
	}

	void Pass::SetSinkLink(std::string registeredName, std::string target)
	{
		auto& sink = GetSink(registeredName);
		sink.SetTarget(target);
	}

	void Pass::Finalise()
	{
		for (auto& sink : m_Sinks)
		{
			sink->ValidateLinks();
		}
		for (auto& source : m_Sources)
		{
			source->ValidateLinks();
		}

		AT_CORE_ASSERT(!(!m_RenderTarget && !m_DepthBuffer), "The pass {0} needs either a render target or a depth buffer", m_Name)
	}

	void Pass::RegisterSink(std::unique_ptr<Sink> sink)
	{
		for (auto& ownedSink : m_Sinks)
		{
			AT_CORE_ASSERT(!(ownedSink->GetRegisteredName() == sink->GetRegisteredName()), "The sinks {0} already exists in pass {1}", sink->GetRegisteredName(), m_Name)
		}

		m_Sinks.push_back(std::move(sink));
	}

	void Pass::RegisterSource(std::unique_ptr<Source> source)
	{
		for (auto& ownedSource : m_Sources)
		{
			AT_CORE_ASSERT(!(ownedSource->GetRegisteredName() == source->GetRegisteredName()), "The source {0} already exists in pass {1}", source->GetRegisteredName(), m_Name)
		}

		m_Sources.push_back(std::move(source));
	}
}