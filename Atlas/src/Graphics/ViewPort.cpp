#include "pch.h"
#include "ViewPort.h"

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
	
	void ViewPort::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(1, &m_ViewPort));
	}

	void ViewPort::MultiBind(std::vector<ViewPort> viewports)
	{
		uint numViewports = (uint)viewports.size();

		D3D11_VIEWPORT* rawPorts = new D3D11_VIEWPORT[numViewports];
		for (uint i = 0; i < numViewports; i++)
		{
			rawPorts[i] = viewports[i].GetViewPort();
		}

		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(numViewports, rawPorts))
	}

	D3D11_VIEWPORT ViewPort::GetViewPort()
	{ 
		return m_ViewPort; 
	}
}