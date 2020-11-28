#include "pch.h"
#include "Technique.h"
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	Technique::Technique(std::string name, bool startActive)
		: m_Name(std::move(name)), m_Active(startActive) { }

	void Technique::Submit(const Drawable& drawable)
	{
		if(m_Active)
			for (Step& step : m_Steps)
			{
				step.Submit(drawable);
			}
	}

	void Technique::Link(RenderGraph& renderGraph)
	{
		for (Step& step : m_Steps)
		{
			step.Link(renderGraph);
		}
	}
}