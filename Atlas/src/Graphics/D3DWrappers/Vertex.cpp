#include "pch.h"
#include "Vertex.h"

namespace Atlas
{
	//These adds the attributes to the descriptor
	//And increment the stride
	void Vertex::AddAttribute(std::string name, uint type, int size)
	{
		m_VertexDescriptor.push_back({ name, type });
		m_Stride += size;
	}

	void Vertex::AddAttribute(std::tuple<std::string, uint, int> vertexData)
	{
		auto [name, type, size] = vertexData;
		m_VertexDescriptor.push_back({ name, type });
		m_Stride += size;
	}
	
	//This adds the data to the memory
	//If too little was given, the application is interrupted
	//And the user is informed of the issue
	void Vertex::PushVertexData(void* data)
	{
		byte* unformattedData = (byte*)data;
		for (int i = 0; i < m_Stride; i++)
		{
			try
			{
				m_Data.push_back(unformattedData[i]);
			}
			catch (std::exception&)
			{
				AT_CORE_ASSERT_WARG(false,"You gave too little data, only {0}, of the {1} bytes could be extracted", i, m_Stride);
			}
		}
	}

	//Returns the Input layout
	std::shared_ptr<InputLayout> Vertex::GetInputLayout(wrl::ComPtr<ID3DBlob> blob, std::string tag)
	{
		return std::move(InputLayout::Create(m_VertexDescriptor, blob, tag));
	}

	//Returns the vertex buffer
	std::shared_ptr<VertexBuffer> Vertex::GetVertexBuffer(std::string tag)
	{
		return std::move(VertexBuffer::Create(m_Data.data(), m_Data.size(), m_Stride, tag));
	}
}
