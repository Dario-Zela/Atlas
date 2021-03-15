#include "pch.h"
#include "Vertex.h"

namespace Atlas
{
	//These adds the attributes to the descriptor
	//And increment the stride
	void Vertex::AddAttribute(const std::string& name, uint type, int size)
	{
		AT_CORE_ASSERT(!m_Finalised, "The vertex has been finalised, attributes cannot be added")
		m_VertexDescriptor.push_back({ name, type });
		m_Stride += size;
	}

	void Vertex::AddAttribute(std::tuple<std::string, uint, int> vertexData)
	{
		AT_CORE_ASSERT(!m_Finalised, "The vertex has been finalised, attributes cannot be added")
		auto [name, type, size] = vertexData;
		m_VertexDescriptor.push_back({ name, type });
		m_Stride += size;
	}
	
	//This adds the data to the memory
	//If too little or too much was given, the application is interrupted
	//And the user is informed of the issue
	void Vertex::PushVertexData(void* data)
	{
		AT_CORE_ASSERT(m_Finalised, "The vertex has not been finalised, data cannot be added")

		AT_CORE_ASSERT(data, "The data was empty")

		byte* unformattedData = (byte*)data;

		//Insert the data
		for (int i = 0; i < m_Stride; i++)
		{
			try
			{
				m_Data.push_back(unformattedData[i]);
			}
			//If there is too little, throw an exception
			catch (std::exception&)
			{
				AT_CORE_ASSERT_WARG(false, "You gave too little data, only {0}, of the {1} bytes could be extracted", i, m_Stride);
			}
		}
	}

	//Returns the Input layout
	std::shared_ptr<InputLayout> Vertex::GetInputLayout(wrl::ComPtr<ID3DBlob> blob, std::string tag)
	{
		return std::move(InputLayout::Create(m_VertexDescriptor, blob, tag));
	}

	//Returns the vertex buffer
	std::shared_ptr<VertexBuffer> Vertex::GetVertexBuffer(const std::string& tag)
	{
		return std::move(VertexBuffer::Create(m_Data.data(), m_Data.size(), m_Stride, tag));
	}
}
