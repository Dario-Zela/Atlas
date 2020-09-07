#include "pch.h"
#include "Exception.h"

namespace Atlas
{
	AtlasException::AtlasException(int line, const char* file) noexcept
		:m_Line(line), m_File(file)
	{
	}

	const char* AtlasException::what() const noexcept
	{
		//This saves the stream to the buffer as
		//The reference would die once the address is returned
		std::stringstream ss;
		ss << GetType() << std::endl
			<< GetOriginString();
		m_Buffer = ss.str();
		return m_Buffer.c_str();
	}

	const char* AtlasException::GetType() const noexcept
	{
		return "Default Atlas Exception";
	}

	int AtlasException::GetLine() const noexcept
	{
		return m_Line;
	}

	const std::string& AtlasException::GetFile() const noexcept
	{
		return m_File;
	}

	std::string AtlasException::GetOriginString() const noexcept
	{
		//Here there is no need for a buffer as the 
		//std::string is returned
		std::stringstream ss;
		ss << "[File]: " << m_File << std::endl
			<< "[Line]: " << m_Line;
		return ss.str();
	}
}