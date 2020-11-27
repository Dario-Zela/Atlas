#include "pch.h"
#include "Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"

namespace Atlas
{
	Sink::Sink(std::string registeredName, std::shared_ptr<Buffer> target)
		: m_Linked(false), m_RegisteredName(registeredName), m_Target(std::move(target))
	{
		AT_CORE_ASSERT(!m_RegisteredName.empty(), "The name of the sink was empty")

			bool valid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

		if (!valid || std::isdigit(registeredName.front()))
			AT_CORE_ASSERT(false, "The sink name {0} is invalid", m_RegisteredName);
	}

	void Sink::SetTarget(std::string sourceName)
	{
		AT_CORE_ASSERT(!(sourceName == "." || sourceName.empty()), "The source name was empty for sink {0}", m_RegisteredName);

		std::vector<std::string> names;
		{
			size_t start;
			size_t end = 0;

			while ((start = sourceName.find_first_not_of('.', end)) != std::string::npos)
			{
				end = sourceName.find('.', start);
				names.push_back(sourceName.substr(start, end - start));
			}
		}

		if (names.size() != 2)
			AT_CORE_ASSERT(false, "The source name was invalid for sink {0}", m_RegisteredName);

		{
			std::string testName(sourceName);
			testName.erase(std::remove(testName.begin(), testName.end(), '.'), testName.end());
			bool valid = std::all_of(testName.begin(), testName.end(), [](char c) {return std::isalnum(c) || c == '_'; });

			if (names[0] != "$" && (!valid || std::isdigit(names[0].front()) || std::isdigit(names[1].front())))
				AT_CORE_ASSERT(false, "The source name was invalid for sink {0}", m_RegisteredName);
		}

		m_PassName = names[0];
		m_OutputName = names[1];
	}

	void Sink::ValidateLinks()
	{
		if (!m_Linked)
		{
			AT_CORE_ASSERT(false, "The sink {0} was not linked to a source", m_RegisteredName)
		}
	}

	void Sink::Bind(Source& source)
	{
		std::shared_ptr<Buffer>& targetSink = source.GetBuffer();

		AT_CORE_ASSERT(targetSink, "The sink for source {0} does not had data", m_RegisteredName)

		AT_CORE_ASSERT(targetSink->GetTypeName() == m_Target->GetTypeName(), "The types of the targets are not equivalent")

		m_Target->Copy(targetSink);
		m_Linked = true;
	}

	std::unique_ptr<Sink> Sink::Create(std::string registeredName, std::shared_ptr<Buffer> target)
	{
		return std::make_unique<Sink>(std::move(registeredName), std::move(target));
	}
}