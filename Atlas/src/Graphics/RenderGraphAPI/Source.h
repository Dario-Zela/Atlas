#pragma once

namespace Atlas
{
	class Buffer;

	class Source
	{
	public:
		Source(std::string registeredName, std::shared_ptr<Buffer> data);
		void ValidateLinks();

		std::string& GetRegisteredName() { return m_RegisteredName; }

		std::shared_ptr<Buffer>& GetBuffer();

		static std::unique_ptr<Source> Create(std::string registeredName, std::shared_ptr<Buffer> data);
	private:
		bool m_Linked;
		std::string m_RegisteredName;
		std::shared_ptr<Buffer> m_Data;
	};
}
