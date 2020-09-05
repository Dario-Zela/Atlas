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
			:mWidth(width), mHeight(height) {};

		//Returns the current width and height of the client rectangle/window
		inline uint GetWidth() { return mWidth; }
		inline uint GetHeight() { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: Width = " << mWidth << ", Heght = " << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		uint mWidth, mHeight;
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