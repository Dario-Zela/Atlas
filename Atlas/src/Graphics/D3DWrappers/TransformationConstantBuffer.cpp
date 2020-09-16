#include "pch.h"
#include "TransformationConstantBuffer.h"

namespace Atlas
{
	TransformationConstantBuffer::TransformationConstantBuffer(Drawable& parent)
		:m_Parent(parent)
	{
		m_VertexBuffer.Create(sizeof(DirectX::XMMATRIX));
	}

	void TransformationConstantBuffer::SetProjection(DirectX::XMMATRIX projection)
	{
		m_Projection = projection;
	}

	void TransformationConstantBuffer::Bind()
	{
		auto mat = DirectX::XMMatrixTranspose(m_Parent.GetTransformXM() * m_Projection);
		m_VertexBuffer.Update((void*)&mat, sizeof(mat));
		m_VertexBuffer.Bind();

	}
}