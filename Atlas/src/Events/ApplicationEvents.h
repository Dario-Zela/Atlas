#pragma once
#include "pch.h"
#include "Events/Event.h"

namespace Atlas
{
	//A window resize event
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint width, uint height)
			:m_Width(width), m_Height(height) {};

		//Returns the current width and height of the client rectangle/window
		inline uint GetWidth() { return m_Width; }
		inline uint GetHeight() { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: Width = " << m_Width << ", Heght = " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		uint m_Width, m_Height;
	};

	//A window closed event
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowClosedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClosed)
	};
}