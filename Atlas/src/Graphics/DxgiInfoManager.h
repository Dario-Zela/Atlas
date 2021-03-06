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
		DxgiInfoManager() = default;
		void Init();
		~DxgiInfoManager() = default;

		//Set is called to filter messages from this point
		static void Set() noexcept;
		
		//This gets the messages taken from the last Set call to now
		static std::vector<std::tuple<std::string, int>> GetMessages();
	private:

		static DxgiInfoManager* s_Instance;
		static unsigned long long s_Next;
		static Microsoft::WRL::ComPtr<IDXGIInfoQueue> s_DxgiInfoQueue;
	};
}
