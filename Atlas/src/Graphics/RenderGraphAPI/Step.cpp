#include "pch.h"
#include "Step.h"
#include "Graphics/Drawable.h"
#include "Graphics/RenderGraphAPI/RenderGraph.h"
#include "Graphics/RenderGraphAPI/RenderQueuePass.h"
#include "Graphics/RenderGraphAPI/Job.h"

namespace Atlas
{
	Step::Step(std::string passName)
		: m_PassName(std::move(passName)) { }

	void Step::Submit(Drawable& drawable)
	{
		//Send a job to the pass
		m_Pass->Accept(Job{ &drawable, this});
	}

	void Step::Link(RenderGraph& renderGraph)
	{
		//Check that the name is not empty
		AT_CORE_ASSERT(!(m_PassName.empty()), "You didn't set the name of the pass")
		//Get it and save a reference to it
		m_Pass = &renderGraph.GetRenderQueue(m_PassName);
	}

	void Step::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Bind all of the bindables to the context
		for (auto& bindable : m_Bindables)
		{
			bindable->Bind(context);
		}
	}

	void Step::ImmidiateBind()
	{
		//Bind all the bindables to the immediate context
		for (auto& bindable : m_Bindables)
		{
			bindable->ImmidiateBind();
		}
	}
}