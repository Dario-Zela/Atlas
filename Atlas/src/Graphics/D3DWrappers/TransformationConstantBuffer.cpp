#include "pch.h"
#include "TransformationConstantBuffer.h"

namespace Atlas
{
	TransformationConstantBuffer::TransformationConstantBuffer(Drawable& parent, DirectX::XMMATRIX projection)
		:m_Parent(parent), m_VertexBuffer(nullptr)
	{
		//Set the vertex constant buffer and the projection matrix
		m_VertexBuffer = std::make_unique<VertexConstantBuffer>((uint)sizeof(DirectX::XMMATRIX));
		m_Projection = projection;
	}

	std::shared_ptr<TransformationConstantBuffer> TransformationConstantBuffer::Create(Drawable& parent, DirectX::XMMATRIX projection)
	{
		//This simply creates the shared ptr as they are unique elements
		//And should never be cloned
		return std::make_shared<TransformationConstantBuffer>(parent, projection);
	}

	void TransformationConstantBuffer::Bind()
	{
		//Get's the parent's transformation and adds it to it's projection
		auto mat = DirectX::XMMatrixTranspose(m_Parent.GetTransformXM() * m_Projection);
		//Updates the constant buffer
		m_VertexBuffer->Update((void*)&mat, sizeof(mat));
		//Binds the buffer
		m_VertexBuffer->Bind();
	}
}