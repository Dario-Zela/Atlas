#include "pch.h"
#include "DX11Exception.h"

namespace Atlas
{
	DX11Exception::DX11Exception(int line, const char* file, HRESULT hr) noexcept
		: AtlasException(line, file), hr(hr)
	{
	}

	const char* DX11Exception::what() const noexcept
	{
		std::stringstream ss;
		ss << GetType() << std::endl
			<< "[ErrorCode]: " << GetErrorCode() << std::endl
			<< "[Description]: " << GetErrorString() << std::endl
			<< GetOriginString();
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
		//If the lenght is 0, the function failed
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
}