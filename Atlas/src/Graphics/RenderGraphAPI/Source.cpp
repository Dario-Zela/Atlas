#include "pch.h"
#include "Source.h"
#include "Graphics/Buffer.h"
#include "Graphics/RenderGraphAPI/Sink.h"

namespace Atlas
{
	Source::Source(std::string registeredName, std::shared_ptr<Buffer> data)
		: m_Linked(false), m_RegisteredName(registeredName), m_Data(std::move(data))
	{
		//Check that there is a name
		AT_CORE_ASSERT(!m_RegisteredName.empty(), "The name of the source was empty")

		//Make sure that there are no special characters 
		bool valid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

		//If it isn't valid or its fist character is a number, throw an exception
		if (!valid || std::isdigit(registeredName.front()))
			AT_CORE_ASSERT_WARG(false, "The source name {0} is invalid", m_RegisteredName);
	}

	void Source::ValidateLinks()
	{
		//If the source is not linked, throw an exception
		if (!m_Linked)
		{
			AT_CORE_ASSERT_WARG(false, "The source {0} was not linked to a sink", m_RegisteredName)
		}
	}

	std::shared_ptr<Buffer>& Source::GetBuffer()
	{
		//The buffer will only be taken by the sink for linking
		//And will always link or throw an exception, so the source should assume this
		
		//Make sure the sources is not already linked
		AT_CORE_ASSERT_WARG(!m_Linked, "You cannot link a source twice. The source was {0}", m_RegisteredName)
		
		//Update it's state
		m_Linked = true;
		//Return the data
		return m_Data;
	}

	std::unique_ptr<Source> Source::Create(std::string registeredName, std::shared_ptr<Buffer> data)
	{
		return std::unique_ptr<Source>(new Source(std::move(registeredName), std::move(data)));
	}
}

