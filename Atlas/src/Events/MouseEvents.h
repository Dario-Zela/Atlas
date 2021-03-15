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
			: m_MouseX(x), m_MouseY(y) { }

		//Returns the new positions of the mouse
		inline uint GetMouseX() const { return m_MouseX; }
		inline uint GetMouseY() const { return m_MouseY; }

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: X = " << m_MouseX << ", Y = " << m_MouseY;
			return *&ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)

	protected:
		uint m_MouseX, m_MouseY;
	};

	//A mouse scrolled event
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			:m_XOffset(xOffset), m_YOffset(yOffset) { }

		//Stores vertical and horizontal deltas
		inline float GetMouseXOffset() const { return m_XOffset; }
		inline float GetMouseYOffset() const { return m_YOffset; }

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: XOffset = " << m_XOffset << ", YOffset = " << m_YOffset;
			return *&ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)

	protected:
		float m_XOffset, m_YOffset;
	};

	//A Mouse button superclass to reduce the function declarations of the implementations
	class MouseButtonEvent : public Event
	{
	public:
		//Returns the button value that has been pressed
		inline int GetMouseButton() const { return m_ButtonPressed; }

	protected:
		MouseButtonEvent(int button)
			:m_ButtonPressed(button) { }

		int m_ButtonPressed;
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

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_ButtonPressed << ", is Double click: " << (m_DoubleClick ? "True" : "False");
			return *&ss.str();
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

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_ButtonPressed;
			return *&ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}