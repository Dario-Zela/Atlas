#include "pch.h"
#include "ViewPort.h"
#include "Graphics/BindableLib.h"

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	ViewPort::ViewPort(float x, float y, float width, float height, float zMin, float zMax)
	{
		m_ViewPort = {};
		m_ViewPort.TopLeftX = x;
		m_ViewPort.TopLeftY = y;
		m_ViewPort.Width = width;
		m_ViewPort.Height = height;
		m_ViewPort.MinDepth = zMin;
		m_ViewPort.MaxDepth = zMax;
	}

	std::shared_ptr<ViewPort> ViewPort::Create(float x, float y, float width, float height, float zMin, float zMax)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID((uint)x, (uint)y, (uint)width, (uint)height, (uint)zMin, (uint)zMax);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<ViewPort>(test);
		}
		//else create a viewport and add it to the library before returning it
		else
		{
			auto viewPort = std::make_shared<ViewPort>(x, y, width, height, zMin, zMax);
			BindableLib::Add(UID, viewPort);
			return std::static_pointer_cast<ViewPort>(BindableLib::Resolve(UID));
		}
	}

	std::string ViewPort::GenerateUID(uint x, uint y, uint width, uint height, uint zMin, uint zMax)
	{
		return std::string(typeid(ViewPort).name()) + '_' + std::to_string(x)
			+ '_' + std::to_string(y) + '_' + std::to_string(width)
			+ '_' + std::to_string(height) + '_' + std::to_string(zMin) + '_' + std::to_string(zMax);
	}
	
	void ViewPort::Bind()
	{
		//Binds the viewport
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(1, &m_ViewPort));
	}
}