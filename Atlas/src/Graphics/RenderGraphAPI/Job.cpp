#include "pch.h"
#include "Job.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	Job::Job(Drawable* drawable, Step* step)
		:m_Drawable(drawable), m_Step(step) { AT_CORE_ASSERT(drawable, "The drawable is empty") AT_CORE_ASSERT(step, "The step is empty") }

	void Job::Execute(wrl::ComPtr<ID3D11DeviceContext> context) const
	{
		//Bind the variables to the deferred context
		m_Drawable->Bind(context);
		m_Step->Bind(context);
		//And execute a draw call
		Graphics::DrawIndexed(m_Drawable->GetIndexCount(), context);
	}

	void Job::ExecuteImmediate() const
	{
		//Bind the variables to the immediate context
		m_Drawable->ImmediateBind();
		m_Step->ImmediateBind();
		//And execute a draw call
		Graphics::ImmediateDrawIndexed(m_Drawable->GetIndexCount());
	}
}