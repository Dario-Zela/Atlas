#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Drawable.h"
#include "Graphics/D3DWrappers/Buffers.h"

namespace Atlas
{
	//A buffer that transforms the mesh between calls
	class TransformationConstantBuffer : public Bindable
	{
	public:
		//Constructor, it takes it's parent as well as the projection matrix
		TransformationConstantBuffer(Drawable& parent, DirectX::XMMATRIX projection);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<TransformationConstantBuffer> Create(Drawable& parent, DirectX::XMMATRIX projection);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		std::unique_ptr<VertexConstantBuffer> m_VertexBuffer;
		Drawable& m_Parent;
		DirectX::XMMATRIX m_Projection;
	};
}