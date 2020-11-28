#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	class DeferredRenderContext
	{
	public:
		DeferredRenderContext();
		wrl::ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }

	private:
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
	};
}