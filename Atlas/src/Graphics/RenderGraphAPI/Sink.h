#pragma once
#include "Graphics/Buffer.h"

namespace Atlas
{
	class Source;

	class Sink
	{
	public:
		Sink(std::string registeredName, std::shared_ptr<Buffer> target);

		void SetTarget(std::string sourceName);
		void ValidateLinks();

		void Bind(Source& source);

		std::string& GetRegisteredName() { return m_RegisteredName; }
		std::string& GetPassName() { return m_PassName; }
		std::string& GetOutputName() { return m_OutputName; }

		static std::unique_ptr<Sink> Create(std::string registeredName, std::shared_ptr<Buffer> target);
	private:
		bool m_Linked;
		std::string m_RegisteredName;
		std::string m_PassName;
		std::string m_OutputName;
		std::shared_ptr<Buffer> m_Target;
	};
}