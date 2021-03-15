#pragma once

namespace Atlas
{
	//Define the needed classes that will be used later
	class Buffer;
	class Sink;

	//A source is the output of a pass that 
	//Can be used by other passes in a render
	//graph
	class Source
	{
		friend Sink;
	public:
		//Creates a source. Needs a name to identify it and a reference to the data buffer.
		//The name must not have a digit as the fist value and contain no special characters 
		//except for _
		static std::unique_ptr<Source> Create(const std::string& registeredName, std::shared_ptr<Buffer> data);

		//Makes sure that the buffer is linked
		void ValidateLinks();

		//Gets the registered name
		std::string& GetRegisteredName() { return m_RegisteredName; }

	private:
		//Creates a source, it requires a name that sources will link to and
		//the data buffer that will be linked to the source's
		Source(const std::string& registeredName, std::shared_ptr<Buffer> data);

		//Gets the buffer and links the source
		std::shared_ptr<Buffer>& GetBuffer();

		//Is the source linked
		bool m_Linked;
		//The name of the source
		std::string m_RegisteredName;
		//The data of the source
		std::shared_ptr<Buffer> m_Data;
	};
}
