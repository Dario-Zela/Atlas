#include "pch.h"
#include "Source.h"
#include "Graphics/Buffer.h"
#include "Graphics/RenderGraphAPI/Sink.h"

namespace Atlas
{
	Source::Source(std::string registeredName, std::shared_ptr<Buffer> data)
		: m_Linked(false), m_RegisteredName(registeredName), m_Data(std::move(data))
	{
		AT_CORE_ASSERT(!m_RegisteredName.empty(), "The name of the source was empty")

			bool valid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

		if (!valid || std::isdigit(registeredName.front()))
			AT_CORE_ASSERT(false, "The source name {0} is invalid", m_RegisteredName);
	}

	void Source::ValidateLinks()
	{
		if (!m_Linked)
		{
			AT_CORE_ASSERT(false, "The source {0} was not linked to a sink", m_RegisteredName)
		}
	}

	std::shared_ptr<Buffer>& Source::GetBuffer()
	{
		AT_CORE_ASSERT(!m_Linked, "You cannot link a source twice. The source was {0}", m_RegisteredName)
		m_Linked = true;
		return m_Data;
	}

	std::unique_ptr<Source> Source::Create(std::string registeredName, std::shared_ptr<Buffer> data)
	{
		return std::make_unique<Source>(std::move(registeredName), std::move(data));
	}
}

