#include "pch.h"
#include "FullScreenPass.h"
#include <DirectXMath.h>
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	FullScreenPass::FullScreenPass(std::string name)
		:Pass(std::move(name))
	{
		//Make a 2D plane
		//I did not use the plane from the SimpleMeshConstructors
		//As it is 3D object and needs all of the positions to be re-cast 
		//Anyway
		std::vector<DirectX::XMFLOAT2> positions;
		positions.push_back(DirectX::XMFLOAT2{ -1, 1 });
		positions.push_back(DirectX::XMFLOAT2{  1, 1 });
		positions.push_back(DirectX::XMFLOAT2{ -1,-1 });
		positions.push_back(DirectX::XMFLOAT2{  1,-1 });

		//Add the vertex buffer
		AddBindable(VertexBuffer::Create(positions.data(), positions.size() * sizeof(DirectX::XMFLOAT2), sizeof(DirectX::XMFLOAT2), "FullScreenPositions"));
		//Make the indices
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		//Add the index buffer
		AddBindable(IndexBuffer::Create(indices.data(), indices.size() * sizeof(unsigned short), "FullScreenPositionIndex"));
	}

	void FullScreenPass::Execute(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Bind all of the bindables and the 
		//Render target and depth buffer
		BindAll(context);

		//Execute a draw call 
		Graphics::DrawIndexed(6, context);
	}

	void FullScreenPass::ExecuteImmidiate()
	{
		//Bind all of the bindables and the 
		//Render target and depth buffer
		BindAll();

		//Execute a draw call 
		Graphics::ImmidiateDrawIndexed(6);
	}
}