#include "pch.h"
#include "Vertex.h"

namespace Atlas 
{
	void Vertex::AddPropriety(std::string semanticName, ElementType type)
	{
		m_Layout.push_back({ semanticName, (DXGI_FORMAT)type });
		m_Size += ElementTypeToStride(type);
	}
}