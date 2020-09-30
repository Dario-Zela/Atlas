#include "pch.h"
#include "Test.h"
#include "Graphics/D3DWrappers/SimpleMeshCreator.h"
#include "Graphics/D3DWrappers/Texture.h"
#include "Graphics/D3DWrappers/Sampler.h"

#include "Graphics/D3DWrappers/Blendable.h"

Atlas::Box::Box(std::mt19937& rng, std::uniform_real_distribution<float> adist, std::uniform_real_distribution<float> ddist, std::uniform_real_distribution<float> odist, std::uniform_real_distribution<float> rdist)
    : r(rdist(rng)), drool(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(odist(rng))
{
	struct Vector : IMovable
	{
		struct {
			float x;
			float y;
		} texCoords;
	};

	///*
	auto [val, tag] = GetRandomIndipendent(rng);
	//*/
	val.Transform(DirectX::XMMatrixScaling(2, 2, 2));

	std::vector<Vector> vec;

	/*
	auto val = Cube::MakeIndipendentFaces();
	std::string tag = "Cube";
	*/

	for (auto v : val.GetVertecies())
	{
		vec.push_back({ v.pos, { (v.pos.x + 1) / 2, (v.pos.y + 1) / 2 } });
	}

	AddBindable(VertexBuffer::Create(vec.data(), vec.size() * (uint)sizeof(Vector), (uint)sizeof(Vector), tag));

	//AddBindable(Texture::Create(R"(assets\Textures\Test2.png)"));

	if(rng() %2 == 1)
		AddBindable(Texture::Create(R"(assets\Textures\Test2.png)"));
	else
		AddBindable(Texture::Create(R"(assets\Textures\Test.bmp)"));

	AddBindable(Sampler::Create());

	AddBindable(Blendable::Create(true, 0));

	auto temp2 = VertexShader::Create("TestVertex.cso");
	auto temp = temp2->GetBlob();
	AddBindable(std::move(temp2));

	AddBindable(PixelShader::Create("TestPixel.cso"));

	AddBindable(IndexBuffer::Create(val.GetIndecies().data(), (uint)val.GetIndeciesSize() * sizeof(unsigned short), tag));

	AddBindable(InputLayout::Create({
		{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT},
		{"TEXTURECOORDS", DXGI_FORMAT_R32G32_FLOAT}
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