#pragma once
#include "Events/Event.h"
#include "Layer/LayerHolder.h"

namespace Atlas
{
	//An event manager that simplifies the event handling
	class EventManager
	{
	public:
		//Initialises the queue
		EventManager() = default;
		~EventManager() = default;

		//This adds an event to the queue
		void AddEventToQueue(Event* e);

		//This handles how the events are given to the layers
		void PropagateEvents(LayerStack* stack);

	private:
		//A queue that stores all the event that occur in one update
		std::queue<Event*> m_EventBacklog;
	};

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