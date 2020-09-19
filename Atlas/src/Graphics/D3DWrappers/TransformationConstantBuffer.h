#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Drawable.h"
#include "Graphics/D3DWrappers/Buffers.h"

namespace Atlas
{
	class TransformationConstantBuffer : public Bindable
	{
	public:
		TransformationConstantBuffer(Drawable& parent);
		void SetProjection(DirectX::XMMATRIX projection);
		void Bind() override;
	private:
		std::unique_ptr<VertexConstantBuffer> m_VertexBuffer;
		Drawable& m_Parent;
		DirectX::XMMATRIX m_Projection;
	};
}