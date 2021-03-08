#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	//A wrapper over a d3d11 deferred context
	class DeferredRenderContext
	{
	public:
		//Constructs an empty deffered context
		DeferredRenderContext();
		//Gets the context
		wrl::ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }

	private:
		//The d3d11 context
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
	};
}