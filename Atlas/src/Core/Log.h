#pragma once
#include "Core.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include "../vendor/fmt/fmt.h"
#include <mutex>

namespace Atlas 
{
	class Log
	{
	public:
		//If the data is to be added to a text file
		//The Init function must be called to initialise it
		void Init(std::string filePos);

		template<typename ...Args>
		void Add(std::string Warning, int WarningLevel, std::string Name, const std::string& fmt, const Args& ...args)
		{
			//Lock the mutex
			m_Mutex.lock();

			//This switches on the warning level to change the colour
			//Of the console
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (WarningLevel)
			{
			case 0: SetConsoleTextAttribute(hStdOut, 7); break;
			case 1: SetConsoleTextAttribute(hStdOut, 2); break;
			case 2: SetConsoleTextAttribute(hStdOut, 14); break;
			case 3: SetConsoleTextAttribute(hStdOut, 12); break;
			case 4: SetConsoleTextAttribute(hStdOut, 4); break;
			}

			//Using the fmt library I can format the string
			std::string string = fmt::format("[" + GetTime() + "]" + "[" + Name + "]"+ "[" + Warning + "] " + fmt, args...);

			std::cout << string << std::endl;

			//If the file-path has been initialised
			if (m_File != "")
			{
				//The file is open and the string is written
				std::ofstream file(m_File, std::ofstream::app);
				file << string + "\n";
				file.close(); //The file is then closed
			}

			//The colour of the console is reset to the original value
			SetConsoleTextAttribute(hStdOut, 15);

			//Unlock the mutex
			m_Mutex.unlock();
		}

	private:
		//Wraps a series of chrono function to get the time
		std::string GetTime();
		//The file-path is store in the string
		std::string m_File = "";
		//A mutex to avoid conflicts
		std::mutex m_Mutex;
	};

	//This allows the log class to be called from the macros
	static Log* Logger = new Log();
}
