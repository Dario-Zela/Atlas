#include "pch.h"
#include "DeferredRenderContext.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	DeferredRenderContext::DeferredRenderContext()
	{
		//Create the context
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDeferredContext(0, &m_Context));
	}
}