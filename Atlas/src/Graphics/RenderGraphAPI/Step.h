#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	//Define the needed classes that will be used later
	class Drawable;
	class Bindable;
	class RenderQueuePass;
	class RenderGraph;

	//A set of bindables that will be used alongside the ones of a drawable
	//They will modify how the data of the drawable is processed
	//Can be used to archive effects such as outlines
	class Step
	{
	public:
		//The name of the pass that is going to be linked to the step
		Step(std::string passName);
		
		//Submits a job to the pass with drawable that the step will act on
		void Submit(Drawable& drawable);
		//Gets a reference to the pass the step will act on
		void Link(RenderGraph& renderGraph);
		//Adds the bindables that will act on the drawable
		void AddBindable(std::shared_ptr<Bindable> bindable) { m_Bindables.push_back(bindable); }
		
		//Binds the bindables to the pipelines
		//To a deferred context
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
		//To the immediate context
		void ImmidiateBind();
	private:
		//The buffer of bindables
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		//The name of the pass that the step links to
		std::string m_PassName;
		
		//The pass that the step links to
		RenderQueuePass* m_Pass;
	};
}