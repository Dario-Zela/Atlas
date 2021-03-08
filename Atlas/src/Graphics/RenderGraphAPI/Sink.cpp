#include "pch.h"
#include "Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"

namespace Atlas
{
	Sink::Sink(std::string registeredName, std::shared_ptr<Buffer> target)
		: m_Linked(false), m_RegisteredName(registeredName), m_Target(std::move(target))
	{
		//Check that there is a name
		AT_CORE_ASSERT(!m_RegisteredName.empty(), "The name of the sink was empty")

		//Make sure that there are no special characters 
		bool valid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

		//If it isn't valid or its fist character is a number, throw an exception
		if (!valid || std::isdigit(registeredName.front()))
			AT_CORE_ASSERT_WARG(false, "The sink name {0} is invalid", m_RegisteredName);
	}

	void Sink::SetTarget(std::string sourceName)
	{
		//Make sure the name of the source is not empty of a full stop.
		AT_CORE_ASSERT_WARG(!(sourceName == "." || sourceName.empty()), "The source name was empty for sink {0}", m_RegisteredName);

		//Divide the source name into the name of the pass and the name of the source
		std::vector<std::string> names;
		{
			size_t start;
			size_t end = 0;

			//This divition is done by a full stop
			while ((start = sourceName.find_first_not_of('.', end)) != std::string::npos)
			{
				end = sourceName.find('.', start);
				names.push_back(sourceName.substr(start, end - start));
			}
		}

		//If there are more then 2 names, throw an exception
		if (names.size() != 2)
			AT_CORE_ASSERT_WARG(false, "The source name was invalid for sink {0}", m_RegisteredName);

		//Make sure the name is valid
		{
			//Remove the dot
			std::string testName(sourceName);
			testName.erase(std::remove(testName.begin(), testName.end(), '.'), testName.end());
			//And then check if all of the values are alphanumeric
			bool valid = std::all_of(testName.begin(), testName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

			//If the pass is not global and the names are not valid or the first character of either name is a digit
			//Throw an exeption
			if (names[0] != "$" && (!valid || std::isdigit(names[0].front()) || std::isdigit(names[1].front())))
				AT_CORE_ASSERT_WARG(false, "The source name was invalid for sink {0}", m_RegisteredName);
		}

		//Assign the names
		m_PassName = names[0];
		m_SourceName = names[1];
	}

	void Sink::ValidateLinks()
	{
		//If the sink is not linked, throw an exeption
		if (!m_Linked)
		{
			AT_CORE_ASSERT_WARG(false, "The sink {0} was not linked to a source", m_RegisteredName)
		}
	}

	void Sink::Bind(Source& source)
	{
		//Get the buffer of the source
		std::shared_ptr<Buffer>& targetSink = source.GetBuffer();

		//If the buffer is null, throw an exeption
		AT_CORE_ASSERT_WARG(targetSink, "The sink for source {0} does not have data", m_RegisteredName)

		//If the the targets do not match in type, throw an exeption
		AT_CORE_ASSERT(targetSink->GetTypeName() == m_Target->GetTypeName(), "The types of the targets are not equivalent")

		//Copy the pointer to the data
		m_Target->Equate(targetSink);
		//Update it's state
		m_Linked = true;
	}

	std::unique_ptr<Sink> Sink::Create(std::string registeredName, std::shared_ptr<Buffer> target)
	{
		return std::unique_ptr<Sink>(new Sink(std::move(registeredName), std::move(target)));
	}
}