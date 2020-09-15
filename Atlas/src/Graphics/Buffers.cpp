#include "pch.h"
#include "Buffers.h"

namespace Atlas
{
	//Vertex Buffer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(void* data, uint sizeData, uint sizeVertex)
	{
		//Creates the descriptor
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = sizeVertex;

		//Sets the data
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		//Creates the buffer
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer))
	}

	void VertexBuffer::Bind(uint stride, uint offset)
	{
		//Binds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset))
	}

	void VertexBuffer::Multibind(std::vector<VertexBuffer> buffers, const uint* strides, const uint* offsets)
	{
		uint numBuffers = (uint)buffers.size();

		//Creates a raw array of buffers
		ID3D11Buffer** rawBuffers = new ID3D11Buffer*[numBuffers];
		for (uint i = 0; i < numBuffers; i++)
		{
			rawBuffers[i] = buffers[i].GetVertexBuffer().Get();
		}

		//Binds them all
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetVertexBuffers(0, numBuffers, rawBuffers, strides, offsets))

		//And deletes the raw array
		delete[] rawBuffers;
	}

	wrl::ComPtr<ID3D11Buffer> VertexBuffer::GetVertexBuffer()
	{
		return m_VertexBuffer;
	}

	//Index Buffer
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(unsigned short* data, uint size)
	{
		//Creates the descriptor
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = size;
		bufferDesc.StructureByteStride = sizeof(unsigned short);

		//Sets the data
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = data;

		//Creates the buffer
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &indexData, &m_IndexBuffer))
	}

	void IndexBuffer::Bind(uint offset)
	{
		//Binds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, offset))
	}

	void IndexBuffer::Multibind(std::vector<IndexBuffer> buffers, const uint* strides, const uint* offsets)
	{
		uint numBuffers = (uint)buffers.size();

		//Creates a raw array of buffers
		ID3D11Buffer** rawBuffers = new ID3D11Buffer*[numBuffers];
		for (uint i = 0; i < numBuffers; i++)
		{
			rawBuffers[i] = buffers[i].GetIndexBuffer().Get();
		}

		//Binds them all
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetVertexBuffers(0, numBuffers, rawBuffers, strides, offsets))
		
		//And deletes the raw array
		delete[] rawBuffers;
	}

	wrl::ComPtr<ID3D11Buffer> IndexBuffer::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}

	uint IndexBuffer::GetCount()
	{
		//Gets the descriptor
		D3D11_BUFFER_DESC bufferDesc;
		m_IndexBuffer->GetDesc(&bufferDesc);

		//Returns the element count
		return bufferDesc.ByteWidth / sizeof(unsigned short);
	}

	//Constant Buffer
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ConstantBuffer::ConstantBuffer(void* data, uint sizeData)
	{
		//Creates the descriptor
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeData;
		bufferDesc.StructureByteStride = 0;

		//Sets the data
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		//Creates the buffer
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBuffer(&bufferDesc, &vertexData, &m_VertexBuffer))
	}

	void ConstantBuffer::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetConstantBuffers(0, 1, m_VertexBuffer.GetAddressOf()))
	}

	void ConstantBuffer::Multibind(std::vector<ConstantBuffer> buffers)
	{
		uint numBuffers = (uint)buffers.size();

		//Creates a raw array of buffers
		ID3D11Buffer** rawBuffers = new ID3D11Buffer*[numBuffers];
		for (uint i = 0; i < numBuffers; i++)
		{
			rawBuffers[i] = buffers[i].GetConstantBuffer().Get();
		}

		//Binds them all
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetConstantBuffers(0, numBuffers, rawBuffers))

		//And deletes the raw array
		delete[] rawBuffers;
	}

	wrl::ComPtr<ID3D11Buffer> ConstantBuffer::GetConstantBuffer()
	{
		return wrl::ComPtr<ID3D11Buffer>();
	}
}