#include "pch.h"
#include "EventManager.h"

namespace Atlas
{
	//Adds an event pointer to the queue
	void EventManager::AddEventToQueue(Event* e)
	{
		m_EventBacklog.push(e);
	}

	//Propagates the events through the layer stack
	void EventManager::PropagateEvents(LayerStack* stack)
	{
		while (!m_EventBacklog.empty())
		{
			//Get the event and remove it from the queue
			Event* e = m_EventBacklog.front();
			m_EventBacklog.pop();

			//As it is a stck it is run from back to front
			for (auto it = stack->rbegin(); it != stack->rend(); it++)
			{
				(*it)->OnEvent(*e);
				if (e->IsHandled())			//If the event is handled, 
					break;					//It stops propagating
			}

			//The event is deleted to not leak memory
			delete e;
		}
	}
}
