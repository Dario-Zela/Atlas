#include "pch.h"
#include "Drawable.h"

namespace Atlas 
{
	void Drawable::Draw() const
	{
		for (auto bindable : m_Bindables)
			bindable->Bind();

		Graphics::DrawIndexed(m_IndexBuffer->GetCount());
	}

	void Drawable::AddBindable(Bindable& bindable)
	{
		if (typeid(bindable) == typeid(IndexBuffer))
		{
			AT_ASSERT(!m_IndexBuffer, "Attempted to add the index buffer twice");

			m_Bindables.push_back(&bindable);
			m_IndexBuffer = (IndexBuffer*)&bindable;
		}
		else
		{
			m_Bindables.push_back(&bindable);
		}
	}
}