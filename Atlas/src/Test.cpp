#include "pch.h"
#include "Test.h"

Atlas::Box::Box(std::mt19937& rng, std::uniform_real_distribution<float> adist, std::uniform_real_distribution<float> ddist, std::uniform_real_distribution<float> odist, std::uniform_real_distribution<float> rdist)
    :r(rdist(rng)), drool(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(odist(rng)), tcb(*this)
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	const Vertex vertices[] =
	{
		{{-1.0f, -1.0f, -1.0f}, {255, 0, 0, 255}},
		{{1.0f, -1.0f, -1.0f}, {255, 255, 0, 255}},
		{{-1.0f, 1.0f, -1.0f}, {255, 125, 0, 255}},
		{{1.0f, 1.0f, -1.0f}, {255, 0, 255, 255}},
		{{-1.0f, -1.0f, 1.0f}, {255, 0, 125, 255}},
		{{1.0f, -1.0f, 1.0f}, {255, 255, 125, 255}},
		{{-1.0f, 1.0f, 1.0f}, {255, 125, 255, 255}},
		{{1.0f, 1.0f, 1.0f}, {255, 0, 0, 255}}
	};

	AddBindable(VertexBuffer::Create((void*)&vertices, sizeof(vertices), sizeof(Vertex)));

	auto temp2 = VertexShader::Create("TestVertex.cso");
	auto temp = temp2->GetBlob();
	AddBindable(std::move(temp2));

	AddBindable(PixelShader::Create("TestPixel.cso"));

	const unsigned short indices[] =
	{
		0,2,1,	2,3,1,
		1,3,5,	3,7,5,
		2,6,3,	3,6,7,
		4,5,7,	4,7,6,
		0,4,2,	2,4,6,
		0,1,4,	1,5,4
	};

	AddBindable(IndexBuffer::Create((unsigned short*)&indices, sizeof(indices)));

	AddBindable(InputLayout::Create({
		{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0},
		{"COLOR", DXGI_FORMAT_R8G8B8A8_UNORM,0}
		}, temp));

	Graphics::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::SetRenderTarget();

	tcb.SetProjection(DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 1000));

	AddBindable(ViewPort::Create(0, 0, 1024, 700, 0, 1));
}

void Atlas::Box::Update(float timeStep)
{
	r += 0.5f * dir;
	if (r < -30)
		dir = 1;
	if (r > 20)
		dir = -1;

	pitch += dpitch * timeStep;
	theta += dtheta * timeStep;
	roll += drool * timeStep;
	yaw += dyaw * timeStep ;
	phi += dphi * timeStep ;
	chi += dchi * timeStep ;
	tcb.Bind();
}

DirectX::XMMATRIX Atlas::Box::GetTransformXM()
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 50.0f);
}
