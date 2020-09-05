#pragma once

namespace Atlas
{
	//A helper class that stores the time between frames
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f)
			:mTime(time)
		{
		}

		//This allows it to be implicitly cast into a float
		//Note that the cast is into seconds
		operator float() const { return mTime; }

		//This allows the float value to be taken
		//As seconds or as milliseconds
		inline float GetSeconds() const { return mTime; }
		inline float GetMilliseconds() const { return mTime * 1000.0f; }

	private:
		float mTime;
	};
}