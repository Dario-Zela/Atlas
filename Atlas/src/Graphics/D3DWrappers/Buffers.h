#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include <mutex>

namespace Atlas
{
	//Wraps a d3d11 buffer object for vertex use
	class VertexBuffer : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<VertexBuffer> Create(void* data, uint sizeData, uint sizeVertex, std::string tag);

		//Gets a vertex buffer if it has been previously constructed
		//Returns nullptr otherwise
		static std::shared_ptr<VertexBuffer> Get(std::string tag);

		//Generates the unique identifier for the buffer
		static std::string GenerateUID(std::string tag);
		
		//Binds the buffer
		void ImmidiateBind() override;		
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor, takes in the pointer to the data, the size of the data and the size of a vertex
		VertexBuffer(void* data, uint sizeData, uint sizeVertex);	

		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint m_Stride;
	};

	//Wraps a d3d11 buffer object for index use
	class IndexBuffer : public Bindable
	{
	public:
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
		//Constructor, takes in the pointer to the data, the size of the data
		IndexBuffer(unsigned short* data, uint size);

		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};


	//Constant Buffer types
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Wraps a d3d11 buffer object for use as a constant buffer

	class TransformationConstantBuffer;

	class ConstantBuffer : public Bindable
	{
		friend TransformationConstantBuffer;
	public:
		static std::shared_ptr<ConstantBuffer> Create(void* data, uint sizeData, std::string tag, uint targets = 0, uint slot = 0);
		static std::shared_ptr<ConstantBuffer> Create(uint sizeData, std::string tag, uint targets = 0, uint slot = 0);

		//Gets a vertex buffer if it has been previously constructed
		//Returns nullptr otherwise
		static std::shared_ptr<ConstantBuffer> Get(std::string tag);

		//Generates the unique identifier for the buffer
		static std::string GenerateUID(std::string tag);

		void ImmidiateUpdate(void* data, uint sizeData);		//Updates the constant buffer
		void Update(void* data, uint sizeData, wrl::ComPtr<ID3D11DeviceContext> context);		//Updates the constant buffer

		void ImmidiateBind() override;		//Binds the buffer
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;

	protected:
		//Constructor, takes in the pointer to the data, the size of the data
		ConstantBuffer(void* data, uint sizeData, uint targets, uint slot);
		//Deferred constructor, takes in the size of the data only
		ConstantBuffer(uint sizeData, uint targets, uint slot);

		std::mutex m_Mutex;
		std::vector<std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer* const*)>> m_Binds;
		wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		uint m_Slot;
	};
}
