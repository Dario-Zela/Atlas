#pragma once
#include "pch.h"
#include "Events/Event.h"

namespace Atlas
{
	//A key event superclass
	class KeyEvent : public Event
	{
	public:
		//Returns the key-code of the key
		inline int GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode)
		{
		}

		int m_KeyCode;
	};

	//A key pressed event 
	class KeyPressedEvent : public KeyEvent
	{
	public:
		//It takes the key-code and if the key has been held
		KeyPressedEvent(int keycode, bool held)
			:KeyEvent(keycode), m_Held(held)
		{
		}

		//Returns if the key was held
		inline bool IsHeld() { return m_Held; }

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << ", is Held: " << (m_Held ? "True" : "False");
			return  *&ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_Held;
	};

	//A key released event
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode)
		{
		}

		std::string& ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return  *&ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	//A key-typed event
	class KeyTypedEvent : public Event
	{
	public:
		//It doesn't use the superclass as the key-code
		//Represent the ASCII value of the character
		KeyTypedEvent(int keycode)
			:m_Character(keycode)
		{
		}

		inline char GetKeyCode() const { return m_Character; }

		std::string& ToString() const
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Character;
			return  *&ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	
	private:
		char m_Character;
	};
}