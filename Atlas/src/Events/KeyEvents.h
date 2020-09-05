#pragma once
#include "pch.h"
#include "Events/Event.h"

namespace Atlas
{
	//A key event superclass
	class KeyEvent : public Event
	{
	public:
		//Returns the keycode of the key
		inline int GetKeyCode() const { return mKeyCode; }

	protected:
		KeyEvent(int keycode)
			:mKeyCode(keycode)
		{
		}

		int mKeyCode;
	};

	//A key pressed event 
	class KeyPressedEvent : public KeyEvent
	{
	public:
		//It takes the keycode and if the key has been held
		KeyPressedEvent(int keycode, bool held)
			:KeyEvent(keycode), mHeld(held)
		{
		}

		//Returns if the key was held
		inline bool IsHeld() { return mHeld; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mHeld << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool mHeld;
	};

	//A key released event
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	//A keytyped event
	class KeyTypedEvent : public Event
	{
	public:
		//It doesn't use the superclass as the keycode
		//Rapresent the ASCII value of the character
		KeyTypedEvent(int keycode)
			:m_Character(keycode)
		{
		}

		inline char GetKeyCode() const { return m_Character; }

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Character;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	
	private:
		char m_Character;
	};
}