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
		std::vector<DirectX::XMFLOAT2> positions;
		positions.push_back(DirectX::XMFLOAT2{ -1, 1 });
		positions.push_back(DirectX::XMFLOAT2{  1, 1 });
		positions.push_back(DirectX::XMFLOAT2{ -1,-1 });
		positions.push_back(DirectX::XMFLOAT2{  1,-1 });

		AddBindable(VertexBuffer::Create(positions.data(), positions.size() * sizeof(DirectX::XMFLOAT2), sizeof(DirectX::XMFLOAT2), "FullScreenPositions"));
		std::vector<unsigned short> indices = { 0,1,2,1,3,2 };
		AddBindable(IndexBuffer::Create(indices.data(), indices.size() * sizeof(unsigned short), "FullScreenPositionIndex"));
	}

	void FullScreenPass::Execute(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		BindAll(context);
		Graphics::DrawIndexed(6, context);
	}

	void FullScreenPass::ExecuteImmidiate()
	{
		BindAll();
		Graphics::ImmidiateDrawIndexed(6);
	}
}