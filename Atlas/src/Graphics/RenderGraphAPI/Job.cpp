#include "pch.h"
#include "Job.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	Job::Job(Drawable* drawable, Step* step)
		:m_Drawable(drawable), m_Step(step) { }

	void Job::Execute(wrl::ComPtr<ID3D11DeviceContext> context) const
	{
		//Bind the variables to the deferred context
		m_Drawable->Bind(context);
		m_Step->Bind(context);
		//And execute a draw call
		Graphics::DrawIndexed(m_Drawable->GetIndexCount(), context);
	}

	void Job::ExecuteImmidiate() const
	{
		//Bind the variables to the immediate context
		m_Drawable->ImmidiateBind();
		m_Step->ImmidiateBind();
		//And execute a draw call
		Graphics::ImmidiateDrawIndexed(m_Drawable->GetIndexCount());
	}
}