#include "pch.h"
#include "TransformationConstantBuffer.h"
#include "Targets.h"

namespace Atlas
{
	TransformationConstantBuffer::TransformationConstantBuffer(Drawable& parent, DirectX::XMMATRIX& projection, uint slot)
		:m_Parent(parent), m_VertexBuffer(nullptr)
	{
		//Set the vertex constant buffer and the projection matrix
		m_VertexBuffer = std::unique_ptr<ConstantBuffer>(new ConstantBuffer((uint)sizeof(DirectX::XMMATRIX), (uint)TargetShader::VertexShader, slot));
		m_Projection = projection;
	}

	std::shared_ptr<TransformationConstantBuffer> TransformationConstantBuffer::Create(Drawable& parent, DirectX::XMMATRIX projection, uint slot)
	{
		//This simply creates the shared ptr as they are unique elements
		//And should never be cloned
		return std::shared_ptr<TransformationConstantBuffer>(std::move(new TransformationConstantBuffer(parent, projection, slot)));
	}

	void TransformationConstantBuffer::ImmediateBind()
	{
		//Get's the parent's transformation and adds it to it's projection
		auto mat = DirectX::XMMatrixTranspose(m_Parent.GetTransform() * m_Projection);
		//Updates the constant buffer
		m_VertexBuffer->ImmediateUpdate((void*)&mat, sizeof(mat));
		//Binds the buffer
		m_VertexBuffer->ImmediateBind();
	}

	void TransformationConstantBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Get's the parent's transformation and adds it to it's projection
		auto mat = DirectX::XMMatrixTranspose(m_Parent.GetTransform() * m_Projection);
		//Updates the constant buffer
		m_VertexBuffer->Update((void*)&mat, sizeof(mat), context);
		//Binds the buffer
		m_VertexBuffer->Bind(context);
	}
}