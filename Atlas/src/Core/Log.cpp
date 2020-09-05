#include "pch.h"
#include "Log.h"

namespace Atlas
{
	void Log::Init(std::string filePos)
	{
		//The file position is saved
		m_File = filePos;
		try
		{
			//The file position is tested by creating a bookmark of the current date and time
			std::ofstream stream(m_File, std::ofstream::app);
			if (!stream.fail())
				throw std::exception();
			time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			tm time = *std::localtime(&now);
			stream << "\n\n" + std::to_string(time.tm_mday) + "\\" + std::to_string(time.tm_mon + 1) + "\\" + std::to_string(time.tm_year + 1900) + " "
				+ std::to_string(time.tm_hour) + ":" + std::to_string(time.tm_min) + ":" + std::to_string(time.tm_sec) + "\n"
				+ "--------------------------------------------\n";
			stream.close();
		}
		catch (std::exception&)		//If it fails, the application is killed and an error is reported
		{
			AT_ASSERT(false, "The log file was unaccessible,/n either because it doesn't exist /nor because it is incorrect");
		}
	}

	//This get the current time in a string format
	std::string Log::GetTime()
	{
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm time = *std::localtime(&now);
		return std::to_string(time.tm_hour) + ":" + std::to_string(time.tm_min) + ":" + std::to_string(time.tm_sec);
	}
}