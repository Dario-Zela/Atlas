#include "pch.h"
#include "Test.h"
#include "Graphics/D3DWrappers/SimpleMeshCreator.h"
#include "Graphics/D3DWrappers/Texture.h"
#include "Graphics/D3DWrappers/Sampler.h"

#include "Graphics/D3DWrappers/Blendable.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Atlas::Box::Box(std::mt19937& rng, std::uniform_real_distribution<float> adist, std::uniform_real_distribution<float> ddist, std::uniform_real_distribution<float> odist, std::uniform_real_distribution<float> rdist)
    : r(rdist(rng)), drool(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(odist(rng))
{
	struct Color
	{
		byte r;
		byte g;
		byte b;
		byte a;
	};

	struct Vector
	{
		DirectX::XMFLOAT3 pos;
		Color col;
	};

	std::vector<Vector> vec;
	auto tag = "Model";

	Assimp::Importer imp;
	const auto model = imp.ReadFile(R"(assets\Models\201906_KeyVR_Webinar_Scenes\201906_KeyVR_BoatCollection.ksp)", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);
	//201906_KeyVR_Webinar_Scenes\201906_KeyVR_BoatCollection.ksp
	
	const auto mesh = model->mMeshes[0];
	vec.reserve(mesh->mNumVertices);

	Color color = { (byte)(rng() % 256), (byte)(rng() % 256), (byte)(rng()%256), 255 };
	
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		vec.push_back({
			{ mesh->mVertices[i].x * 2, mesh->mVertices[i].y * 2, mesh->mVertices[i].z * 2 },
			color });
			
		color = { (byte)(rng() % 256), (byte)(rng() % 256), (byte)(rng() % 256), 255 };
	}

	AddBindable(VertexBuffer::Create(vec.data(), vec.size() * (uint)sizeof(Vector), (uint)sizeof(Vector), tag));

	std::vector<unsigned short> indicies;
	indicies.reserve(mesh->mNumFaces * 3);

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		indicies.push_back(face.mIndices[0]);
		indicies.push_back(face.mIndices[1]);
		indicies.push_back(face.mIndices[2]);
	}

	auto temp2 = VertexShader::Create("TestVertex.cso");
	auto temp = temp2->GetBlob();
	AddBindable(std::move(temp2));

	AddBindable(PixelShader::Create("TestPixel.cso"));

	AddBindable(IndexBuffer::Create(indicies.data(), (uint)indicies.size() * sizeof(unsigned short), tag));

	AddBindable(InputLayout::Create({
		{"POSITION", DXGI_FORMAT_R32G32B32_FLOAT},
		{"COLOR", DXGI_FORMAT_R8G8B8A8_UNORM}
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