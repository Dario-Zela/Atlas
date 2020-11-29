#pragma once
#include <DirectXMath.h>
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/RenderGraphAPI/Technique.h"

namespace Atlas
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;

		void ImmidiateBind() const;
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) const;
		void Submit();

		void AddBindable(std::shared_ptr<Bindable> bindable);
		void ClearBindables() { m_Bindables.clear(); m_IndexBuffer = nullptr; }

		void LinkTechniques(RenderGraph& renderGraph);

		uint GetIndexCount() const { return m_IndexBuffer->GetCount(); }

		void AddTechnique(Technique& technique) { m_Techniques.push_back(technique); }

		virtual DirectX::XMMATRIX GetTransformXM() = 0;
	private:
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer = nullptr;
		std::vector<Technique> m_Techniques;
	};
}