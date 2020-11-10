#include "pch.h"
#include "Drawable.h"
#include "Graphics/D3DWrappers/ViewPort.h"

namespace Atlas 
{
	void Drawable::Draw() const
	{
		for (auto bindable : m_Bindables)
			bindable->Bind();

		Graphics::DrawIndexed(m_IndexBuffer->GetCount());
	}

	void Drawable::AddBindable(std::shared_ptr<Bindable> bindable)
	{
		if (typeid(*bindable) == typeid(IndexBuffer))
		{
			AT_ASSERT(!m_IndexBuffer, "Attempted to add the index buffer twice");

			m_IndexBuffer = (IndexBuffer*)bindable.get();
			m_Bindables.push_back(std::move(bindable));
		}
		else
		{
			if (typeid(*bindable) == typeid(ViewPort))
				auto x = 0;
			m_Bindables.push_back(bindable);
		}
	}
}