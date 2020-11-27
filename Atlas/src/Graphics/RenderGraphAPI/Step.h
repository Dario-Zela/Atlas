#pragma once

namespace Atlas
{
	class Drawable;
	class Bindable;
	class RenderQueuePass;
	class RenderGraph;

	class Step
	{
	public:
		Step(std::string passName);
		void Submit(const Drawable& drawable);
		void Link(RenderGraph& renderGraph);
		void AddBindable(std::shared_ptr<Bindable> bindable) { m_Bindables.push_back(std::move(bindable)); }
		void Bind();

	private:
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		std::string m_PassName;
		RenderQueuePass* m_Pass;
	};
}