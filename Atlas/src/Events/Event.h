#pragma once

namespace Atlas
{
	//An enum to compare different EventTypes
	enum class EventType
	{
		Undefined = 0,
		WindowClosed, WindowResize, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		Custom
	};

	//This simplifies a series of overrides
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual std::string GetName() const override { return #type; }

	//The abstract event superclass
	class Event
	{
	public:
		//Gets the event type to compare the different types
		virtual EventType GetEventType() const = 0;
		
		//For standard events, these are debug only
		//Custom classes will use them to differentiate which type is what
		virtual std::string GetName() const = 0;
		virtual std::string& ToString() const = 0;
		
		//Allows the user to handle the event and stop it
		//from passing to the next layer
		inline bool HandleEvent() { m_Handled = true; }
		
		//Returns if the event has been handled
		inline bool IsHandled() const { return m_Handled; }
	protected:
		bool m_Handled = false;
	};
}
