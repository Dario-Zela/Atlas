#pragma once
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/InputLayout.h"

namespace Atlas
{
	//A class that abstracts the creation of a vertex buffer
	class Vertex
	{
	public:
		Vertex() = default;
		
		//This adds an attribute to the vertex
		//This can be done as a set of values or as a single tuple
		void AddAttribute(const std::string& name, uint type, int size);
		void AddAttribute(std::tuple<std::string, uint, int> vertexData);

		//This pushes the data of one vertex into memory
		void PushVertexData(void* data);

		//Finalises the structure of the Vertex
		//Must be done before data is pushed into the vertex
		void Finalise() { m_Finalised = true; }

		//These function get the input layout and vertex buffer
		//Abstracting the creation of these elements
		std::shared_ptr<InputLayout> GetInputLayout(wrl::ComPtr<ID3DBlob> blob, std::string tag);
		std::shared_ptr<VertexBuffer> GetVertexBuffer(const std::string& tag);
	private:
		std::vector<InputElement> m_VertexDescriptor;
		std::vector<byte> m_Data;
		int m_Stride;
		bool m_Finalised;
	};
}