#pragma once
#include <DirectXMath.h>
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/RenderGraphAPI/Technique.h"

namespace Atlas
{
	//Define classes used later
	class Job;

	class Drawable
	{
		friend Job;
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;
		
		//Submits the drawable, through it's techniques
		//to the linked render graph
		void Submit();
		//Draws the drawable without using any technique
		//to the back buffer
		void Draw();

		//Links the techniques of the drawable to a render graph
		void LinkTechniques(RenderGraph& renderGraph);

		//The transform of the object form the origin
		virtual DirectX::XMMATRIX GetTransformXM() = 0;

	protected:
		//Adds a technique to be used by the render graph
		void AddTechnique(Technique& technique) { m_Techniques.push_back(technique); }

		//Gets the index count of the drawable
		uint GetIndexCount() const { return m_IndexBuffer->GetCount(); }

		//Adds a resource to be used during the drawing
		void AddBindable(std::shared_ptr<Bindable> bindable);
		//Clears the buffer of bindables
		void ClearBindables() { m_Bindables.clear(); m_IndexBuffer = nullptr; }
		
		//The storage of resources
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
	private:
		//Binds the drawable to the immediate context
		void ImmidiateBind() const;
		//Binds the drawable to the deferred context
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) const;

		//A reference to the index buffer
		IndexBuffer* m_IndexBuffer = nullptr;
		//The storage of techniques
		std::vector<Technique> m_Techniques;
	};
}