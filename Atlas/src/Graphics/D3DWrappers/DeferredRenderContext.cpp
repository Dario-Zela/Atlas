#include "pch.h"
#include "DeferredRenderContext.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	DeferredRenderContext::DeferredRenderContext()
	{
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDeferredContext(0, &m_Context));
	}
}