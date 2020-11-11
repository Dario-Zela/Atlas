#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	//Wraps a d3d11 buffer object for vertex use
	class VertexBuffer : public Bindable
	{
	public:
		//Constructor, takes in the pointer to the data, the size of the data and the size of a vertex
		VertexBuffer(void* data, uint sizeData, uint sizeVertex);	
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<VertexBuffer> Create(void* data, uint sizeData, uint sizeVertex, std::string tag);

		//Gets a vertex buffer if it has been previously constructed
		//Returns nullptr otherwise
		static std::shared_ptr<VertexBuffer> Get(std::string tag);

		//Generates the unique identifier for the buffer
		static std::string GenerateUID(std::string tag);
		void Bind() override;		//Binds the buffer
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint m_Stride;
	};

	//Wraps a d3d11 buffer object for index use
	class IndexBuffer : public Bindable
	{
	public:
		//Constructor, takes in the pointer to the data, the size of the data
		IndexBuffer(unsigned short* data, uint size);
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<IndexBuffer> Create(unsigned short* data, uint size, std::string tag);

		//Gets a vertex buffer if it has been previously constructed
		//Returns nullptr otherwise
		static std::shared_ptr<IndexBuffer> Get(std::string tag);

		//Generates the unique identifier for the buffer
		static std::string GenerateUID(std::string tag);
		void Bind() override;		//Binds the buffer
		uint GetCount() const;		//Gets the number of elements
	private:
		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};


	//Constant Buffer types
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Wraps a d3d11 buffer object for use as a constant buffer
	class ConstantBuffer : public Bindable
	{
	public:
		//Constructor, takes in the pointer to the data, the size of the data
		ConstantBuffer(void* data, uint sizeData);
		//Deferred constructor, takes in the size of the data only
		ConstantBuffer(uint sizeData);

		//The wrappers over the constructors to get a shared ptr
		static std::shared_ptr<ConstantBuffer> Create(void* data, uint sizeData);
		static std::shared_ptr<ConstantBuffer> Create(uint sizeData);

		void Update(void* data, uint sizeData);		//Updates the constant buffer

		void Bind() override {};		//Binds the buffer
	protected:
		wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	//A constant buffer used in the vertex shader
	class VertexConstantBuffer : public ConstantBuffer
	{
	public:
		//The constructors are wrapper over the general constant buffer constructors
		VertexConstantBuffer(void* data, uint sizeData) : ConstantBuffer(data, sizeData) {}
		VertexConstantBuffer(uint sizeData) : ConstantBuffer(sizeData) {}

		void Bind() override;		//This binds the buffer to the vertex shader
	};

	//A constant buffer used in the pixel shader
	class PixelConstantBuffer : public ConstantBuffer
	{
	public:
		//The constructors are wrapper over the general constant buffer constructors
		PixelConstantBuffer(void* data, uint sizeData) : ConstantBuffer(data, sizeData) {}
		PixelConstantBuffer(uint sizeData) : ConstantBuffer(sizeData) {}

		void Bind() override;		//This binds the buffer to the pixel shader
	};
}
