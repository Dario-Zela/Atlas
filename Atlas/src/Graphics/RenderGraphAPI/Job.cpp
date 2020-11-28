#include "pch.h"
#include "Job.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	Job::Job(const Drawable* drawable, Step* step)
		:m_Drawable(drawable), m_Step(step) { }

	void Job::Execute(wrl::ComPtr<ID3D11DeviceContext> context) const
	{
		m_Drawable->Bind(context);
		m_Step->Bind(context);
		Graphics::DrawIndexed(m_Drawable->GetIndexCount(), context);
	}

	void Job::ExecuteImmidiate() const
	{
		m_Drawable->ImmidiateBind();
		m_Step->ImmidiateBind();
		Graphics::ImmidiateDrawIndexed(m_Drawable->GetIndexCount());
	}
}