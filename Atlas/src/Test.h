#pragma once
#include "pch.h"
#include "Graphics/Drawable.h"
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/InputLayout.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/D3DWrappers/ViewPort.h"
#include "Graphics/D3DWrappers/TransformationConstantBuffer.h"

namespace Atlas
{
	class Box : public Drawable
	{
	public:
		Box(std::mt19937& rng,
			std::uniform_real_distribution<float> adist,
			std::uniform_real_distribution<float> ddist,
			std::uniform_real_distribution<float> odist,
			std::uniform_real_distribution<float> rdist);
		void Update(float timeStep) override;
		DirectX::XMMATRIX GetTransformXM() override;
	private:
		//Pos
		float r;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float theta;
		float phi;
		float chi;
		//Speed
		float drool;
		float dpitch;
		float dyaw;
		float dtheta;
		float dphi;
		float dchi;
		int dir;

		//Values
		VertexBuffer vb;
		IndexBuffer ib;
		VertexShader vs;
		PixelShader ps;
		VertexConstantBuffer vcb;
		InputLayout il;
		TransformationConstantBuffer tcb;
		ViewPort vp;
	};
}