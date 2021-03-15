#pragma once
#include "Graphics/Buffer.h"

namespace Atlas
{
	//Define the needed classes that will be used later
	class Source;

	//This is an input into a pass from an other pass
	//They can only transfer buffers, i.e.. the depth stencil buffer
	//and the render target
	//To get a bindable, get the 
	class Sink
	{
	public:
		//Creates a sink. Needs a name to identify it and a reference to the target buffer.
		//The name must not have a digit as the fist value and contain no special characters 
		//except for _
		static std::unique_ptr<Sink> Create(const std::string& registeredName, std::shared_ptr<Buffer> target);

		//Sets the target source
		void SetTarget(const std::string& sourceName);
		//Makes sure it is linked
		void ValidateLinks();

		//Binds the source to the sink
		void Bind(Source& source);

		//Gets the full name of the sink
		std::string& GetRegisteredName() { return m_RegisteredName; }
		//Gets the name of the pass
		std::string& GetPassName() { return m_PassName; }
		//Gets the name of the source it is linked to
		std::string& GetSourceName() { return m_SourceName; }
	private:
		//Creates a sink, it requires a name that sources will link to and
		//the target buffer that will be linked to the source's
		Sink(const std::string& registeredName, std::shared_ptr<Buffer> target);

		//Is the sink linked
		bool m_Linked;
		//The full name of the sink
		std::string m_RegisteredName;
		//The name of the pass
		std::string m_PassName;
		//The name of the buffer
		std::string m_SourceName;
		//The buffer that is being linked
		std::shared_ptr<Buffer> m_Target;
	};
}