#include "pch.h"
#include "Drawable.h"
#include "Graphics/D3DWrappers/ViewPort.h"
#include "Graphics/Drawable.h"
#include "Graphics/RenderGraphAPI/RenderGraph.h"

namespace Atlas 
{
	void Drawable::ImmediateBind() const
	{
		//Immediately bind all of the bindables
		for (auto& bindable : m_Bindables)
			bindable->ImmediateBind();
	}

	void Drawable::Bind(wrl::ComPtr<ID3D11DeviceContext> context) const
	{
		//Bind all of the bindables to the context
		for (auto& bindable : m_Bindables)
			bindable->Bind(context);
	}

	void Drawable::Submit()
	{
		AT_CORE_ASSERT(m_Linked, "The drawable/scene has not been linked to a render graph")

		//Submit all techniques with reference to the drawable
		for (auto& techique : m_Techniques)
			techique.Submit(*this);
	}

	void Drawable::Draw() 
	{ 
		//Immediately bind all of the bindables
		ImmediateBind(); 
		//The immediately draw to the back buffer
		Graphics::ImmediateDrawIndexed(m_IndexBuffer->GetCount()); 
	}

	void Drawable::AddBindable(std::shared_ptr<Bindable> bindable)
	{
		AT_CORE_ASSERT(bindable, "The bindable was empty")

		//If the bindables being added is an index buffer
		if (typeid(*bindable) == typeid(IndexBuffer))
		{
			//Make sure there is no index buffer already
			AT_CORE_ASSERT(!m_IndexBuffer, "Attempted to add the index buffer twice");

			//Link it to the index buffer variable
			m_IndexBuffer = (IndexBuffer*)bindable.get();
			//And add it to the bindables
			m_Bindables.push_back(std::move(bindable));
		}
		else
		{
			//Else add it directly to the buffer
			m_Bindables.push_back(bindable);
		}
	}

	void Drawable::LinkTechniques(RenderGraph& renderGraph)
	{
		m_Linked = true;

		//Link all techniques to the render graph
		for (auto& techique : m_Techniques)
			techique.Link(renderGraph);
	}
}