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
		void ImmidiateBind() override;		//Binds the buffer
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
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
		void ImmidiateBind() override;		//Binds the buffer
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;

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
		ConstantBuffer(void* data, uint sizeData, uint slot);
		//Deferred constructor, takes in the size of the data only
		ConstantBuffer(uint sizeData, uint slot);

		void ImmidiateUpdate(void* data, uint sizeData);		//Updates the constant buffer
		void Update(void* data, uint sizeData, wrl::ComPtr<ID3D11DeviceContext> context);		//Updates the constant buffer

		void ImmidiateBind() override {};		//Binds the buffer
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override {}
	protected:
		wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		uint m_Slot;
		bool mod = false;
	};

	//A constant buffer used in the vertex shader
	class VertexConstantBuffer : public ConstantBuffer
	{
	public:
		//The constructors are wrapper over the general constant buffer constructors
		VertexConstantBuffer(void* data, uint sizeData, uint slot = 0) : ConstantBuffer(data, sizeData, slot) {}
		VertexConstantBuffer(uint sizeData, uint slot = 0) : ConstantBuffer(sizeData, slot) {}
		
		static std::shared_ptr<VertexConstantBuffer> Create(void* data, uint sizeData, uint slot = 0) { return std::make_shared<VertexConstantBuffer>(data, sizeData, slot); }
		static std::shared_ptr<VertexConstantBuffer> Create(uint sizeData, uint slot = 0) { return std::make_shared<VertexConstantBuffer>(sizeData, slot); }

		void ImmidiateBind() override;		//This binds the buffer to the vertex shader
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;

	};

	//A constant buffer used in the pixel shader
	class PixelConstantBuffer : public ConstantBuffer
	{
	public:
		//The constructors are wrapper over the general constant buffer constructors
		PixelConstantBuffer(void* data, uint sizeData, uint slot) : ConstantBuffer(data, sizeData, slot) {}
		PixelConstantBuffer(uint sizeData, uint slot) : ConstantBuffer(sizeData, slot) {}

		static std::shared_ptr<PixelConstantBuffer> Create(void* data, uint sizeData, uint slot = 0) { return std::make_shared<PixelConstantBuffer>(data, sizeData, slot); }
		static std::shared_ptr<PixelConstantBuffer> Create(uint sizeData, uint slot = 0) { return std::make_shared<PixelConstantBuffer>(sizeData, slot); }
		void ImmidiateBind() override;		//This binds the buffer to the pixel shader
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	};
}
