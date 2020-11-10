#include "pch.h"
#include "Buffers.h"
#include "Graphics/BindableLib.h"

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	//Vertex Buffer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(void* data, uint sizeData, uint sizeVertex)
		: m_Stride(sizeVertex)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = sizeVertex;


		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer))
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(void* data, uint sizeData, uint sizeVertex, std::string tag)
	{
		std::string UID = GenerateUID(sizeVertex, tag);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<VertexBuffer>(test);
		}
		else
		{
			auto vertexBuffer = std::make_shared<VertexBuffer>(data, sizeData, sizeVertex);
			BindableLib::Add(UID, vertexBuffer);
			return std::static_pointer_cast<VertexBuffer>(BindableLib::Resolve(UID));
		}
	}

	std::string VertexBuffer::GenerateUID(uint stride, std::string tag)
	{
		return std::string(typeid(VertexBuffer).name()) + '_' + std::to_string(stride) + tag;
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

	IndexBuffer::IndexBuffer(unsigned short* data, uint size)
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

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned short* data, uint size, std::string tag)
	{
		std::string UID = GenerateUID(size, tag);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<IndexBuffer>(test);
		}
		else
		{
			auto indexBuffer = std::make_shared<IndexBuffer>(data, size);
			BindableLib::Add(UID, indexBuffer);
			return std::static_pointer_cast<IndexBuffer>(BindableLib::Resolve(UID));
		}
	}

	std::string IndexBuffer::GenerateUID(uint size, std::string tag)
	{
		return std::string(typeid(IndexBuffer).name()) + '_' + std::to_string(size) + tag;
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

	ConstantBuffer::ConstantBuffer(void* data, uint sizeData)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData + (16 - sizeData % 16);
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_ConstantBuffer))
	}

	ConstantBuffer::ConstantBuffer(uint sizeData)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData + (16 - sizeData % 16);
		bufferDesc.StructureByteStride = 0;

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer))
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(void* data, uint sizeData)
	{
		std::string UID = GenerateUID(sizeData);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<ConstantBuffer>(test);
		}
		else
		{
			auto constantBuffer = std::make_shared<ConstantBuffer>(data, sizeData);
			BindableLib::Add(UID, constantBuffer);
			return std::static_pointer_cast<ConstantBuffer>(BindableLib::Resolve(UID));
		}
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(uint sizeData)
	{
		std::string UID = GenerateUID(sizeData);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<ConstantBuffer>(test);
		}
		else
		{
			auto constantBuffer = std::make_shared<ConstantBuffer>(sizeData);
			BindableLib::Add(UID, constantBuffer);
			return std::static_pointer_cast<ConstantBuffer>(BindableLib::Resolve(UID));
		}
	}

	void ConstantBuffer::Update(void* data, uint sizeData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		AT_CHECK_GFX_INFO(Graphics::GetContext()->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		memcpy(mappedResource.pData, data, sizeData);
		
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->Unmap(m_ConstantBuffer.Get(), 0));
	}

	std::string ConstantBuffer::GenerateUID(uint sizeData)
	{
		return std::string(typeid(ConstantBuffer).name()) + '_' + std::to_string(sizeData);
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