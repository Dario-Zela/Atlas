#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	//A wrapper over a vertex buffer object
	class VertexBuffer
	{
	public:
		VertexBuffer(void* data, uint sizeData, uint sizeVertex);

		//Binds it
		void Bind(uint stride, uint offset);
		//Allows for multiple bindings
		static void Multibind(std::vector<VertexBuffer> buffers, const uint* strides, const uint* offsets);
		//Getter
		wrl::ComPtr<ID3D11Buffer> GetVertexBuffer();
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};

	//A wrapper over an index buffer object
	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned short* data, uint size);

		//Binds it
		void Bind(uint offset);
		//Allows for multiple bindings
		static void Multibind(std::vector<IndexBuffer> buffers, const uint* strides, const uint* offsets);
		//Getter
		wrl::ComPtr<ID3D11Buffer> GetIndexBuffer();
		//Gets the number of elements in the data
		uint GetCount();
	private:
		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};

	//A wrapper over a constant buffer
	class ConstantBuffer
	{
	public:
		ConstantBuffer(void* data, uint sizeData);

		//Binds it
		void Bind();
		//Allows for multiple bindings
		static void Multibind(std::vector<ConstantBuffer> buffers);
		//Getter
		wrl::ComPtr<ID3D11Buffer> GetConstantBuffer();
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};
}
