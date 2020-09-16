#include "pch.h"
#include "Buffers.h"

namespace Atlas
{
	//Vertex Buffer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VertexBuffer::Create(void* data, uint sizeData, uint sizeVertex)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = sizeVertex;

		m_Stride = sizeVertex;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer))
	}

	void VertexBuffer::Bind()
	{
		uint zero = 0;
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &zero))
	}

	wrl::ComPtr<ID3D11Buffer> VertexBuffer::GetVertexBuffer()
	{
		return m_VertexBuffer;
	}

	//Index Buffer
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void IndexBuffer::Create(unsigned short* data, uint size)
	{

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = size;
		bufferDesc.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = data;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer))
	}

	void IndexBuffer::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0))
	}


	wrl::ComPtr<ID3D11Buffer> IndexBuffer::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}

	uint IndexBuffer::GetCount() const
	{
		D3D11_BUFFER_DESC bufferDesc;
		m_IndexBuffer->GetDesc(&bufferDesc);
		return bufferDesc.ByteWidth / sizeof(unsigned short);
	}

	//Constant Buffer
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ConstantBuffer::Create(void* data, uint sizeData)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_ConstantBuffer))
	}

	void ConstantBuffer::Create(uint sizeData)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = 0;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer))
	}

	void ConstantBuffer::Update(void* data, uint sizeData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		AT_CHECK_GFX_INFO(Graphics::GetContext()->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		memcpy(mappedResource.pData, data, sizeData);
		
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->Unmap(m_ConstantBuffer.Get(), 0));
	}

	//Vertex Shader Constant Buffer
	void VertexConstantBuffer::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}

	//Pixel Shader Constant Buffer
	void PixelConstantBuffer::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}
}