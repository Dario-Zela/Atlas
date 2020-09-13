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
		static void Init();
		static void Relese();
		~DxgiInfoManager() = default;

		//Set is called to filter messages from this point
		static void Set() noexcept;
		
		//This gets the messages taken from the last Set call to now
		static std::vector<std::string> GetMessages();
	private:
		DxgiInfoManager() = default;

		static DxgiInfoManager* s_Instance;
		static unsigned long long m_Next;
		static Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
	};
}
