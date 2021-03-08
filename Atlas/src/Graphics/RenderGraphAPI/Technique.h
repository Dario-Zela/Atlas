#pragma once
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	//Define the needed classes that will be used later
	class RenderGraph;
	class Drawable;

	//A technique is a set of steps that will specify how to 
	//draw a drawable. They will usually contain the shaders that the
	//Drawable will use as well as texture's or rasteriser states.
	//They should be on a per pass basis, ie. The in the outline pass it
	//will use the outline technique which will first draw the a solid enlarged shape on a mask
	//and then superimpose it over the regualr shape
	//It is must be made for a drawble that use a render graph
	//or for scenes
	class Technique
	{
	public:
		//A technique will be composed of a debug name and a whether it 
		//start active or not
		Technique(std::string name, bool startActive = true);
		//Sends the drawable data to each step so that they get put into the
		//RenderQueue
		void Submit(Drawable& drawable);
		//Links the steps to the render graph
		void Link(RenderGraph& renderGraph);
		//Adds a step to the technique
		void AddStep(Step& step) { m_Steps.push_back(std::move(step)); }

		//Gets the name of the technique
		std::string& GetName() { return m_Name; }
		//Sets whether a technique is active or not
		void SetActiveState(bool state) { m_Active = state; }
		//Returns the current state of the technique
		bool IsActive() { return m_Active; }
	private:
		//The steps of the technique
		std::vector<Step> m_Steps;
		//The name of the technique
		std::string m_Name;
		//Wether it is active or not
		bool m_Active;
	};
}