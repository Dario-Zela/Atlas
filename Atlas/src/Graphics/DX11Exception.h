#pragma once
#include "Core/Exception.h"

namespace Atlas
{
	//An implementation for normal DX11 Exeptions
	class DX11Exception : public AtlasException
	{
	public:
		DX11Exception(int line, const char* file, HRESULT hr, std::vector<std::string> messages = std::vector<std::string>()) noexcept;
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

#define AT_CHECK_GFX(x) {HRESULT hr = x; if(FAILED(hr)) throw DX11Exception(__LINE__, __FILE__, hr);}