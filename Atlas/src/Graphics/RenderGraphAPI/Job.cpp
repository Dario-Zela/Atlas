#include "pch.h"
#include "Job.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	Job::Job(const Drawable* drawable, Step* step)
		:m_Drawable(drawable), m_Step(step) { }

	void Job::Execute() const
	{
		m_Drawable->Bind();
		m_Step->Bind();
		Graphics::DrawIndexed(m_Drawable->GetIndexCount());
	}
}