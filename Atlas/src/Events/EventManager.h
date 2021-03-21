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
}