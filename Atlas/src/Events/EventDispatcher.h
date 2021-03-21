#pragma once
#include "Events/Event.h"

namespace Atlas
{
	//This dispatches the events from the OnEvent function
	//To the appropriate functions
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:m_Event(event)
		{
		}

		//The dispatch function takes a function and
		//Calls it if the event it was initialised with
		//An the one of the function match
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}