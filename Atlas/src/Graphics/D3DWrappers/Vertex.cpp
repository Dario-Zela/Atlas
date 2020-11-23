#include "pch.h"
#include "Vertex.h"

namespace Atlas
{
	void Vertex::AddAtribute(std::string name, uint type, int size)
	{
		m_VectorDescriptor.push_back({ name, type });
		m_Stride += size;
	}

	void Vertex::AddAtribute(std::tuple<std::string, uint, int> vertexData)
	{
		auto [name, type, size] = vertexData;
		m_VectorDescriptor.push_back({ name, type });
		m_Stride += size;
	}
	
	void Vertex::PushVertexData(void* data)
	{
		byte* unformattedData = (byte*)data;
		for (int i = 0; i < m_Stride; i++)
		{
			m_Data.push_back(unformattedData[i]);
		}
	}
	std::shared_ptr<InputLayout> Vertex::GetInputLayout(wrl::ComPtr<ID3DBlob> blob, std::string tag)
	{
		return std::move(InputLayout::Create(m_VectorDescriptor, blob, tag));
	}

	std::shared_ptr<VertexBuffer> Vertex::GetVertexBuffer(std::string tag)
	{
		return std::move(VertexBuffer::Create(m_Data.data(), m_Data.size(), m_Stride, tag));
	}
}
