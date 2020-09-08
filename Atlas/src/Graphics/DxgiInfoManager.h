#pragma once
#include "pch.h"
#include <wrl.h>
#include <dxgidebug.h>

namespace Atlas
{
	//It get the Debug messages from the console
	//And saves them to a sting vector
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		void Init();
		~DxgiInfoManager() = default;

		//Set is called to filter messages from this point
		void Set() noexcept;
		
		//This gets the messages taken from the last Set call to now
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long m_Next = 0;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
	};
}
