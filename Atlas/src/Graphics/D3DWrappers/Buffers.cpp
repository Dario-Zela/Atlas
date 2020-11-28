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
		//The descriptor for a static vertex buffer 
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = sizeVertex;

		//The data of the vertex buffer
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		//This creates the vertex buffer element
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer))
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(void* data, uint sizeData, uint sizeVertex, std::string tag)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(tag);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<VertexBuffer>(test);
		}
		//else create a vertex buffer and add it to the library before returning it
		else
		{
			auto vertexBuffer = std::make_shared<VertexBuffer>(data, sizeData, sizeVertex);
			BindableLib::Add(UID, vertexBuffer);
			return std::static_pointer_cast<VertexBuffer>(BindableLib::Resolve(UID));
		}
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Get(std::string tag)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(tag);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<VertexBuffer>(test);
		}
		//else return nullptr
		else
		{
			//Log the lack of buffer
			AT_WARN("There is no vertex buffer that uses the tag {0}", tag)
			return nullptr;
		}
	}

	std::string VertexBuffer::GenerateUID(std::string tag)
	{
		return std::string(typeid(VertexBuffer).name()) + '_' + tag;
	}

	void VertexBuffer::ImmidiateBind()
	{
		//This binds the vertex buffer
		uint zero = 0;
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &zero))
	}

	void VertexBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//This binds the vertex buffer
		uint zero = 0;
		AT_CHECK_GFX_INFO_VOID(context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &zero))
	}

	//Index Buffer
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(unsigned short* data, uint size)
	{
		//The descriptor for a static index buffer 
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = size;
		bufferDesc.StructureByteStride = sizeof(unsigned short);

		//The data of the index buffer
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = data;

		//This creates the index buffer element
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer))
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned short* data, uint size, std::string tag)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(tag);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<IndexBuffer>(test);
		}
		//else create a index buffer and add it to the library before returning it
		else
		{
			auto indexBuffer = std::make_shared<IndexBuffer>(data, size);
			BindableLib::Add(UID, indexBuffer);
			return std::static_pointer_cast<IndexBuffer>(BindableLib::Resolve(UID));
		}
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Get(std::string tag)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(tag);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<IndexBuffer>(test);
		}
		//else return nullptr
		else
		{
			//Log the lack of buffer
			AT_WARN("There is no vertex buffer that uses the tag {0}", tag)
				return nullptr;
		}
	}

	std::string IndexBuffer::GenerateUID(std::string tag)
	{
		return std::string(typeid(IndexBuffer).name()) + '_' + tag;
	}

	void IndexBuffer::ImmidiateBind()
	{
		//This binds the index buffer
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0))
	}

	void IndexBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//This binds the index buffer
		AT_CHECK_GFX_INFO_VOID(context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0))
	}

	uint IndexBuffer::GetCount() const
	{
		//Get the buffer descriptor from the index buffer
		D3D11_BUFFER_DESC bufferDesc;
		m_IndexBuffer->GetDesc(&bufferDesc);

		//return the total size devided by the size of an index
		return bufferDesc.ByteWidth / sizeof(unsigned short);
	}

	//Constant Buffer
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ConstantBuffer::ConstantBuffer(void* data, uint sizeData)
	{
		//The descriptor for a static constant buffer 
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData + (16 - sizeData % 16);
		bufferDesc.StructureByteStride = 0;

		//The data of the constant buffer
		D3D11_SUBRESOURCE_DATA constantData;
		constantData.pSysMem = data;

		//This creates the constant buffer element
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &constantData, &m_ConstantBuffer))
	}

	ConstantBuffer::ConstantBuffer(uint sizeData)
	{
		//The descriptor for a static constant buffer 
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData + (16 - sizeData % 16);
		bufferDesc.StructureByteStride = 0;

		//This creates the constant buffer element
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_ConstantBuffer))
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(void* data, uint sizeData)
	{
		//This simply creates the shared ptr as they are unique elements
		//And should never be cloned
		return std::make_shared<ConstantBuffer>(data, sizeData);
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(uint sizeData)
	{
		//This simply creates the shared ptr as they are unique elements
		//And should never be cloned
		return std::make_shared<ConstantBuffer>(sizeData);
	}

	void ConstantBuffer::Update(void* data, uint sizeData)
	{
		//Get access to the buffer element's resources
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		AT_CHECK_GFX_INFO(Graphics::GetContext()->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		//Write over the data
		memcpy(mappedResource.pData, data, sizeData);
		
		//Map the results to the element
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->Unmap(m_ConstantBuffer.Get(), 0));
	}

	//Vertex Shader Constant Buffer
	void VertexConstantBuffer::ImmidiateBind()
	{
		//Binds the element to the vertex shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}

	void VertexConstantBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the element to the vertex shader
		AT_CHECK_GFX_INFO_VOID(context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}

	//Pixel Shader Constant Buffer
	void PixelConstantBuffer::ImmidiateBind()
	{
		//Binds the element to the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}
	void PixelConstantBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the element to the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf()));
	}
}