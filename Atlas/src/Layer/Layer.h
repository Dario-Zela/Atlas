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
		virtual void OnAttach() {}
		//The OnDetach when it is popped
		virtual void OnDetach() {}
		//This is executed each frame
		virtual void OnUpdate(TimeStep time) {}
		//This is a redirector to functions that use
		//The instances of the events
		virtual void OnEvent(Event& e) {}

		//Gets the debug name
		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
