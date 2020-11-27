#pragma once
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	class RenderGraph;
	class Drawable;

	class Technique
	{
	public:
		Technique(std::string name, bool startActive = true);
		void Submit(const Drawable& drawable);
		void Link(RenderGraph& renderGraph);
		void AddStep(Step& step) { m_Steps.push_back(std::move(step)); }
		void Bind();

		std::string& GetName() { return m_Name; }
		void SetActiveState(bool state) { m_Active = state; }
		bool IsActive() { return m_Active; }
	private:
		std::vector<Step> m_Steps;
		std::string m_Name;
		bool m_Active;
	};
}