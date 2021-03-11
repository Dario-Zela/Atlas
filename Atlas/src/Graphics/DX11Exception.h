#pragma once
#include "Core/Exception.h"

namespace Atlas
{
	//An implementation for normal DX11 Exceptions
	class DX11Exception : public AtlasException
	{
	public:
		DX11Exception(int line, const char* file, HRESULT hr, std::vector<std::tuple<std::string, int>>  messages = std::vector<std::tuple<std::string, int>>()) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		//This translates the error result into a string
		//To be used in the what method
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string m_Info;
	};
}

#if AT_DEBUG
#define AT_CHECK_GFX(x) {HRESULT hr = x; if(FAILED(hr)) throw DX11Exception(__LINE__, __FILE__, hr);}
#define AT_CHECK_GFX_INFO(x) DxgiInfoManager::Set(); {HRESULT hr = x; if(FAILED(hr))  throw DX11Exception(__LINE__, __FILE__, hr, DxgiInfoManager::GetMessages());}
#define AT_CHECK_GFX_INFO_VOID(x) DxgiInfoManager::Set(); x; { std::vector<std::tuple<std::string, int>> messages = DxgiInfoManager::GetMessages(); if(messages.size() > 0) for(int i = 0; i < messages.size(); i++) { auto[message, severity] = messages[i]; if(severity < 2) throw DX11Exception(__LINE__, __FILE__, -1, messages); else AT_CORE_WARN(message); }}
#else
#define AT_CHECK_GFX(x) x;
#define AT_CHECK_GFX_INFO(x) x;
#define AT_CHECK_GFX_INFO_VOID(x) x;
#endif