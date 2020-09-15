#include "pch.h"
#include "ViewPort.h"

namespace Atlas
{
	ViewPort::ViewPort(float x, float y, float width, float height, float zMin, float zMax)
	{
		//Constructs the viewport object
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
		//Binds the objects
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(1, &m_ViewPort));
	}

	void ViewPort::MultiBind(std::vector<ViewPort> viewports)
	{
		uint numViewports = (uint)viewports.size();

		//Transforms the vector of wrapped items into a raw array
		D3D11_VIEWPORT* rawPorts = new D3D11_VIEWPORT[numViewports];
		for (uint i = 0; i < numViewports; i++)
		{
			rawPorts[i] = viewports[i].GetViewPort();
		}

		//And binds all of them at once
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetViewports(numViewports, rawPorts))

		//Then deletes the array
		delete[] rawPorts;
	}

	D3D11_VIEWPORT ViewPort::GetViewPort()
	{ 
		return m_ViewPort; 
	}
}