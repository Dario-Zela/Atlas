#pragma once
#include "pch.h"
#include <wrl.h>
#include <dxgidebug.h>

namespace Atlas
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		void Init();
		~DxgiInfoManager() = default;
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long m_Next = 0;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
	};
}
