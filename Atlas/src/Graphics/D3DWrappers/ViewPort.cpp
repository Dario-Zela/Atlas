#include "pch.h"
#include "ViewPort.h"
#include "Graphics/BindableLib.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	ViewPort::ViewPort(float x, float y, float width, float height, float zMin, float zMax)
	{
		AT_CORE_ASSERT(x >= 0, "The x value is less then 0, which is not valid")
		AT_CORE_ASSERT(y >= 0, "The y value is less then 0, which is not valid")
		AT_CORE_ASSERT(width >= 0, "The width value is less then 0, which is not valid")
		AT_CORE_ASSERT(height >= 0, "The height value is less then 0, which is not valid")
		AT_CORE_ASSERT(zMin >= 0, "The zMin value is less then 0, which is not valid")
		AT_CORE_ASSERT(zMin < zMax, "The zMin value is grater then zMax, which is not valid")

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
			auto viewPort = new ViewPort(x, y, width, height, zMin, zMax);
			BindableLib::Add(UID, std::shared_ptr<ViewPort>(std::move(viewPort)));
			return std::static_pointer_cast<ViewPort>(BindableLib::Resolve(UID));
		}
	}

	std::string ViewPort::GenerateUID(uint x, uint y, uint width, uint height, uint zMin, uint zMax)
	{
		return std::string(typeid(ViewPort).name()) + '_' + std::to_string(x)
			+ '_' + std::to_string(y) + '_' + std::to_string(width)
			+ '_' + std::to_string(height) + '_' + std::to_string(zMin) + '_' + std::to_string(zMax);
	}
	
	void ViewPort::ImmediateBind()
	{
		//Binds the viewport
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(1, &m_ViewPort));
	}

	void ViewPort::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the viewport
		AT_CHECK_GFX_INFO_VOID(context->RSSetViewports(1, &m_ViewPort));
	}
}