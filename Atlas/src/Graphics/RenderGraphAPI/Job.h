#pragma once
#include "Graphics/Drawable.h"
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	//A job is a step and a drawable combined
	//This is what is executed on a pass
	class Job
	{
	public:
		//Constructor assigning the drawable and the step
		Job(Drawable* drawable, Step* step);

		//Draws the step and drawable in the deferred context
		void Execute(wrl::ComPtr<ID3D11DeviceContext> context) const;
		//Draws the step and drawable in the immediate context
		void ExecuteImmediate() const;

		//The drawable and step linked to the job
		Drawable* m_Drawable;
		Step* m_Step;
	};
}