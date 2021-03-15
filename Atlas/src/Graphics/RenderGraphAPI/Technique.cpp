#include "pch.h"
#include "Technique.h"
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	//Sets the values
	Technique::Technique(const std::string& name, bool startActive)
		: m_Name(name), m_Active(startActive) { }

	void Technique::Submit(Drawable& drawable)
	{
		//If the technique is active
		if(m_Active)
			//Submit each step
			for (Step& step : m_Steps)
			{
				step.Submit(drawable);
			}
	}

	void Technique::Link(RenderGraph& renderGraph)
	{
		//Link each step to the render graph
		for (Step& step : m_Steps)
		{
			step.Link(renderGraph);
		}
	}
}