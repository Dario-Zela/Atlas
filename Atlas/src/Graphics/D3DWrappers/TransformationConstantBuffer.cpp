#include "pch.h"
#include "TransformationConstantBuffer.h"

namespace Atlas
{
	TransformationConstantBuffer::TransformationConstantBuffer(Drawable& parent, DirectX::XMMATRIX projection)
		:m_Parent(parent), m_VertexBuffer(nullptr)
	{
		m_VertexBuffer = std::make_unique<VertexConstantBuffer>((uint)sizeof(DirectX::XMMATRIX));
		m_Projection = projection;
	}

	std::shared_ptr<TransformationConstantBuffer> TransformationConstantBuffer::Create(Drawable& parent, DirectX::XMMATRIX projection)
	{
		return std::make_shared<TransformationConstantBuffer>(parent, projection);
	}

	void TransformationConstantBuffer::Bind()
	{
		auto mat = DirectX::XMMatrixTranspose(m_Parent.GetTransformXM() * m_Projection);
		m_VertexBuffer->Update((void*)&mat, sizeof(mat));
		m_VertexBuffer->Bind();

	}
}