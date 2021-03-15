#include "pch.h"
#include "DxgiInfoManager.h"
#include "Graphics/DX11Exception.h"
#pragma comment(lib, "dxguid.lib")

namespace Atlas
{
	DxgiInfoManager* DxgiInfoManager::s_Instance = nullptr;
	unsigned long long DxgiInfoManager::s_Next = 0;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> DxgiInfoManager::s_DxgiInfoQueue = nullptr;

	//Initialises the queue
	void DxgiInfoManager::Init()
	{
		AT_CORE_ASSERT(!s_Instance, "A dxgiInfoManager has already been constructed")

		s_Instance = this;

		//Declares the signature of the function DXGIGetDebugInterface so that it can be used later
		//		OUTPUT				FUNCTION NAME			INPUTS
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		//Load the DLL that contains the DXGIGetDebugInterface
		const auto modDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (modDxgiDebug)
			AT_CHECK_GFX(GetLastError());

		//Get the function
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(modDxgiDebug, "DXGIGetDebugInterface")));
		if (DxgiGetDebugInterface)
			AT_CHECK_GFX(GetLastError());

		//Get the queue
		AT_CHECK_GFX(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &s_DxgiInfoQueue));
	}

	void DxgiInfoManager::Set() noexcept
	{
		AT_CORE_ASSERT(s_Instance, "The info manager has been released or \nit is being used before it has been constructed")

		//This sets the current index of the Debug queue
		//So that the only messages that arrive are from this point
		s_Next = s_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}
	
	std::vector<std::tuple<std::string, int>> DxgiInfoManager::GetMessages()
	{
		AT_CORE_ASSERT(s_Instance, "The info manager has been released or \nit is being used before it has been constructed")

		std::vector<std::tuple<std::string, int>> messages;
		auto end = s_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = s_Next; i < end; i++)
		{
			SIZE_T messageLenght;

			//Get the size of the message in bytes
			AT_CHECK_GFX(s_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLenght));

			//Allocate the memory
			//This is done so that it is deleted once it is out of scope
			auto bytes = std::make_unique<byte[]>(messageLenght);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			
			//Get the message and push it to the vector
			AT_CHECK_GFX(s_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLenght))

			messages.emplace_back(std::tuple<std::string, int>{ message->pDescription, message->Severity });
		}
		return messages;
	}
}