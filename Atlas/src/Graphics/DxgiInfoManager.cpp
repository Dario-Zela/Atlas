#include "pch.h"
#include "DxgiInfoManager.h"
#include "Graphics/DX11Exception.h"
#pragma comment(lib, "dxguid.lib")

namespace Atlas
{
	DxgiInfoManager::DxgiInfoManager(){}

	void DxgiInfoManager::Init()
	{
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		//Load the dll that contains the DXGIGetDebugInterface
		const auto modDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (modDxgiDebug)
			AT_CHECK_GFX(GetLastError());

		//Get the address
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(modDxgiDebug, "DXGIGetDebugInterface")));
		if (DxgiGetDebugInterface)
			AT_CHECK_GFX(GetLastError());

		//Get the queue
		AT_CHECK_GFX(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_DxgiInfoQueue));
	}
	
	void DxgiInfoManager::Set() noexcept
	{
		//This sets the current index of the Debug queue
		//So that the only messages that arrive are from this point
		m_Next = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}
	
	std::vector<std::string> DxgiInfoManager::GetMessages() const
	{
		std::vector<std::string> messages;
		auto end = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = m_Next; i < end; i++)
		{
			SIZE_T messageLenght;
			
			//Get the size of the message in bytes
			AT_CHECK_GFX(m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLenght));
			
			//Allocate the memory
			//This is done so that it is deleted once it is out of scope
			auto bytes = std::make_unique<byte[]>(messageLenght);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			
			//Get the message and push it to the vector
			AT_CHECK_GFX(m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLenght))

			messages.emplace_back(message->pDescription);
		}
		return messages;
	}
}