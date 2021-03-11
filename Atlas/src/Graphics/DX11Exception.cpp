#include "pch.h"
#include "DX11Exception.h"

namespace Atlas
{
	DX11Exception::DX11Exception(int line, const char* file, HRESULT hr, std::vector<std::tuple<std::string, int>> messages) noexcept
		: AtlasException(line, file), hr(hr)
	{
		//Add the messages to the info
		for (const auto& message : messages)
		{
			m_Info += std::get<0>(message);
			m_Info.push_back('\n');
		}
		//Remove the final new line if it exists
		if (!m_Info.empty())
			m_Info.pop_back();
	}

	const char* DX11Exception::what() const noexcept
	{
		std::stringstream ss;
		ss << GetType() << std::endl
			<< "[ErrorCode]: " << GetErrorCode() << std::endl
			<< "[Description]: " << GetErrorString() << std::endl;
		if (!m_Info.empty())
			ss << "[Extra Error Info]: " << GetErrorInfo() << std::endl << std::endl;
		ss << GetOriginString();
		m_Buffer = ss.str();
		return m_Buffer.c_str();
	}

	const char* DX11Exception::GetType() const noexcept
	{
		return "Window Exception";
	}

	//This can only be used in Windows 8+
	std::string DX11Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		//The buffer for the message is initialised and then filled
		char* MsgBuf = nullptr;
		DWORD MsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MsgBuf), 0, nullptr);
		//If the length is 0, the function failed
		if (MsgLen == 0)
			return "Unidentified error";
		//Else, the string is saved into a variable and the returned
		std::string errorString = MsgBuf;
		LocalFree(MsgBuf);
		return errorString;
	}

	HRESULT DX11Exception::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string DX11Exception::GetErrorString() const noexcept
	{
		return TranslateErrorCode(hr);
	}

	std::string DX11Exception::GetErrorInfo() const noexcept
	{
		return m_Info;
	}
}