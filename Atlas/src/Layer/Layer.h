#pragma once
#include "Events/Event.h"
#include "Core/TimeStep.h"

namespace Atlas
{
	//An abstraction of a layer
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		//The OnAttach is called when the layer is added
		virtual void OnAttach() = 0;
		//The OnDetach when it is popped
		virtual void OnDetach() = 0;
		//This is executed each frame
		virtual void OnUpdate(TimeStep time) = 0;
		//This is a redirector to functions that use
		//The instances of the events
		virtual void OnEvent(Event& e) = 0;

		//Gets the dubug name
		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
