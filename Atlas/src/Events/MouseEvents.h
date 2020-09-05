#pragma once
#include "pch.h"
#include "Events/Event.h"

namespace Atlas
{
	//A mouse moved event
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(uint x, uint y)
			: mMouseX(x), mMouseY(y) { }

		//Returns the new positions of the mouse
		inline uint GetMouseX() const { return mMouseX; }
		inline uint GetMouseY() const { return mMouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: X = " << mMouseX << ", Y = " << mMouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)

	protected:
		uint mMouseX, mMouseY;
	};

	//A mouse scrolled event
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			:mXOffset(xOffset), mYOffset(yOffset) { }

		//Stores vertical and horizontal deltas
		inline float GetMouseXOffset() const { return mXOffset; }
		inline float GetMouseYOffset() const { return mYOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: XOffset = " << mXOffset << ", YOffset = " << mYOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)

	protected:
		float mXOffset, mYOffset;
	};

	//A Mouse button superclass to reduce the function declerations of the implementations
	class MouseButtonEvent : public Event
	{
	public:
		//Returns the the button value that has been pressed
		inline int GetMouseButton() const { return mButtonPressed; }

	protected:
		MouseButtonEvent(int button)
			:mButtonPressed(button) { }

		int mButtonPressed;
	};

	//A button pressed event
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		//Takes the button value, the modifiers and if it is a double click
		MouseButtonPressedEvent(int button, uint mod, bool doubleClick)
			:MouseButtonEvent(button), m_Mod(mod), m_DoubleClick(doubleClick)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButtonPressed << ", is Double click: " << (m_DoubleClick ? "True" : "False");
			return ss.str();
		}

		//The additional functions are the modifiers and double click getters
		inline uint GetModifiers() const { return m_Mod; }
		inline bool IsDoubleClick() const { return m_DoubleClick; }

		EVENT_CLASS_TYPE(MouseButtonPressed)

	private:
		uint m_Mod;
		bool m_DoubleClick;
	};

	//A mouse button released event
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButtonPressed;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}