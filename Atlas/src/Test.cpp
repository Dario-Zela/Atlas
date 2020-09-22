#include "pch.h"
#include "Test.h"
#include "Graphics/D3DWrappers/SimpleMeshCreator.h"

Atlas::Box::Box(std::mt19937& rng, std::uniform_real_distribution<float> adist, std::uniform_real_distribution<float> ddist, std::uniform_real_distribution<float> odist, std::uniform_real_distribution<float> rdist)
    :r(rdist(rng)), drool(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(odist(rng))
{
	struct Col
	{
		byte r;
		byte g;
		byte b;
		byte a;
	};

	struct Vector : IMovable
	{
		Col color;
	};

	///*
	auto [val, tag] = GetRandom(rng);
	//*/

	/*
	auto val = Sphere::MakeTessalated(3, 3);
	std::string tag = "Sphere";
	*/
	
	val.Transform(DirectX::XMMatrixScaling(2, 2, 2));
	std::vector<Vector> vec;

	uint i = 0;
	Col color = { rng() % 256, rng() % 256, rng() % 256, 255 };
	for (auto v : val.GetVertecies()) 
	{
		vec.emplace_back();
		vec.back().pos = v.pos;
		vec.back().color = color;
		i++;
		if (i == 3)
		{
			i = 0;
			color = { rng() % 256, rng() % 256, rng() % 256, 255 };
		}
	}
	AddBindable(VertexBuffer::Create(vec.data(), (uint)vec.size() * sizeof(Vector), (uint)sizeof(Vector), tag));

	auto temp2 = VertexShader::Create("TestVertex.cso");
	auto temp = temp2->GetBlob();
	AddBindable(std::move(temp2));

	AddBindable(PixelShader::Create("TestPixel.cso"));

	AddBindable(IndexBuffer::Create(val.GetIndecies().data(), (uint)val.GetIndeciesSize() * sizeof(unsigned short), tag));

	AddBindable(InputLayout::Create({
		{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT},
		{"COLOR", DXGI_FORMAT_B8G8R8A8_UNORM}
		}, temp));

	Graphics::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::SetRenderTarget();

	AddBindable(TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 1000)));

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
}

DirectX::XMMATRIX Atlas::Box::GetTransformXM()
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 50.0f);
}
