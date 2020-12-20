#pragma once
#include "Atlas.h"

class Cube : public Atlas::Drawable
{
public:
	Cube(Atlas::Camera* camera, DirectX::XMFLOAT3* lightPos, DirectX::XMFLOAT3* dir)
		: camera(camera), lightPos(lightPos), dir(dir)
	{
		auto vertecies = Atlas::Cube::MakeIndipendentFaces();
		vertecies.SetNormalsIndependentFlat();

		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 nor;
			DirectX::XMFLOAT2 tex;
		};
		std::vector<Vertex> vertex;

		for (int i = 0; i < vertecies.GetNormalVertecies().size(); i++)
		{
			DirectX::XMFLOAT2 tex;
			switch (i % 4)
			{
			case 0:
				tex = { 0,0 };
				break;
			case 1:
				tex = { 0,1 };
				break;
			case 2:
				tex = { 1,0 };
				break;
			case 3:
				tex = { 1,1 };
				break;
			}

			vertex.push_back(Vertex{ vertecies.GetNormalVertecies()[i].position, vertecies.GetNormalVertecies()[i].normal, tex });
		}

		cameraBuff = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 3);

		AddBindable(cameraBuff);

		Dirlight = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 4, 1);

		AddBindable(Dirlight);

		Pointlight = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 5 * 4, 2);

		AddBindable(Pointlight);

		std::vector<DirectX::XMFLOAT4> data;

		data.push_back({ 1.0f, 0.5f, 0.31f,0 });
		data.push_back({ 1.0f, 0.5f, 0.31f, 0 });
		data.push_back({ 0.5f, 0.5f, 0.5f, 32 });

		AddBindable(Atlas::PixelConstantBuffer::Create(data.data(), sizeof(DirectX::XMFLOAT4) * data.size(), 3));

		auto vs = Atlas::VertexShader::Create("CubeVS.cso");
		auto blob = vs->GetBlob();
		AddBindable(std::move(vs));
		AddBindable(Atlas::PixelShader::Create("CubePS.cso"));
		std::vector<Atlas::InputElement> elemnts;
		elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		elemnts.push_back({ "NORMAL", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		elemnts.push_back({ "TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT });
		AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "MeshLayout"));

		AddBindable(Atlas::Texture::Create(R"(C:\Users\Dario\Downloads\container2.png)", false, 0));
		AddBindable(Atlas::Sampler::Create(true, true, 14, 0));

		AddBindable(Atlas::Texture::Create(R"(C:\Users\Dario\Downloads\container2_specular.png)", false, 1));
		AddBindable(Atlas::Sampler::Create(true, true, 14, 1));

		AddBindable(Atlas::VertexBuffer::Create(vertex.data(), vertex.size() * sizeof(Vertex), sizeof(Vertex), "Cube"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Cube"));
		AddBindable(Atlas::TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 0.7f, 1, 100000)));

		pos = {(float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10)};
		auto model = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
		AddBindable(Atlas::VertexConstantBuffer::Create(&model, sizeof(model), 1));

		AddBindable(Atlas::Topology::Create());
	}

	DirectX::XMMATRIX GetTransformXM() override
	{
		std::vector<DirectX::XMFLOAT4> data;

		auto front = camera->GetFront().m128_f32;

		data.push_back({ camera->GetPosition().x, camera->GetPosition().y,camera->GetPosition().z, 0 });
		data.push_back({ front[0], front[1], front[2], 0 });
		data.push_back({ cos(DirectX::XMConvertToRadians(5)), cos(DirectX::XMConvertToRadians(7)), 0, 0 });

		cameraBuff->ImmidiateUpdate(data.data(), sizeof(DirectX::XMFLOAT4) * data.size());

		data.clear();

		data.push_back({ dir->x, dir->y, dir->z, 0 });
		data.push_back({ 0.01f, 0.01f, 0.01f, 0 });
		data.push_back({ 0.5f, 0.5f, 0.5f, 0 });
		data.push_back({ 1.0f, 1.0f, 1.0f, 0 });

		Dirlight->ImmidiateUpdate(data.data(), sizeof(DirectX::XMFLOAT4) * data.size());
		
		data.clear();
		for (int i = 0; i < 4; i++) 
		{
			data.push_back({ lightPos[i].x, lightPos[i].y, lightPos[i].z, 0 });
			data.push_back({ 1.0f,	0.09f,	0.032f ,0 });
			data.push_back({ 0.01f, 0.01f, 0.01f, 0 });
			data.push_back({ 0.5f, 0.5f, 0.5f, 0 });
			data.push_back({ 1.0f, 1.0f, 1.0f, 0 });
		}
		Pointlight->ImmidiateUpdate(data.data(), sizeof(DirectX::XMFLOAT4) * data.size());

		return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * camera->GetTransform();
	}

private:
	Atlas::Camera* camera;
	std::shared_ptr<Atlas::PixelConstantBuffer> cameraBuff;
	std::shared_ptr<Atlas::PixelConstantBuffer> Dirlight;
	std::shared_ptr<Atlas::PixelConstantBuffer> Pointlight;
	DirectX::XMFLOAT3* lightPos;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3* dir;
};

class Light : public Atlas::Drawable
{
public:
	Light(DirectX::XMFLOAT3* lightPos, Atlas::Camera* camera)
		: camera(camera), lightPos(lightPos)
	{
		auto vertecies = Atlas::Cube::Make();
		vertecies.Transform(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f));
		AddBindable(Atlas::VertexBuffer::Create(vertecies.GetVertecies().data(), vertecies.GetVertecies().size() * sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), "Light"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Light"));

		auto vs = Atlas::VertexShader::Create("LightVS.cso");
		auto blob = vs->GetBlob();
		AddBindable(std::move(vs));
		AddBindable(Atlas::PixelShader::Create("LightPS.cso"));
		std::vector<Atlas::InputElement> elemnts;
		elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "LightLayout"));

		AddBindable(Atlas::TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 0.7f, 1, 100000)));
		AddBindable(Atlas::Topology::Create());
	}

	DirectX::XMMATRIX GetTransformXM() override
	{
		return DirectX::XMMatrixTranslation(lightPos->x, lightPos->y, lightPos->z) * camera->GetTransform();
	}

private:
	Atlas::Camera* camera;
	DirectX::XMFLOAT3* lightPos;
};

class Screen : public Atlas::Drawable
{
public:
	Screen()
	{
		auto vertecies = Atlas::Plane::Make();
		AddBindable(Atlas::VertexBuffer::Create(vertecies.GetVertecies().data(), vertecies.GetVertecies().size() * sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), "Plane"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Plane"));

		AddBindable(Atlas::Sampler::Create(0, false, false, 0));
		AddBindable(Atlas::Texture::Create(R"(C:\Users\Dario\Downloads\blending_transparent_window.png)", false));
		AddBindable(Atlas::Blendable::Create(true, 0));

		auto vs = Atlas::VertexShader::Create("WindowVS.cso");
		auto blob = vs->GetBlob();
		AddBindable(std::move(vs));
		AddBindable(Atlas::PixelShader::Create("WindowPS.cso"));
		std::vector<Atlas::InputElement> elemnts;
		elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "PlaneLayout"));

		AddBindable(Atlas::Topology::Create());

		/*
		auto vertecies = Atlas::Plane::Make();
		AddBindable(Atlas::VertexBuffer::Create(vertecies.GetVertecies().data(), vertecies.GetVertecies().size() * sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), "Plane"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Plane"));

		auto vs = Atlas::VertexShader::Create("WindowVS.cso");
		auto blob = vs->GetBlob();
		AddBindable(std::move(vs));
		AddBindable(Atlas::PixelShader::Create("WindowPS.cso"));
		std::vector<Atlas::InputElement> elemnts;
		elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "PlaneLayout"));
		AddBindable(Atlas::TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 0.7f, 1, 100000)));
		AddBindable(Atlas::Topology::Create());
		*/
	}

	DirectX::XMMATRIX GetTransformXM() override
	{
		return DirectX::XMMatrixIdentity();
	}
};

class BrickWall : public Atlas::Drawable
{
public:
	BrickWall(Atlas::Camera* camera, DirectX::XMFLOAT3* lightPos, DirectX::XMFLOAT3* dir)
		: camera(camera), lightPos(lightPos), dir(dir)
	{
		auto vertecies = Atlas::Cube::Make();
		
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};
		std::vector<Vertex> vertex;

		for (int i = 0; i < vertecies.GetVertecies().size(); i++)
		{
			DirectX::XMFLOAT2 tex;
			switch (i % 4)
			{
			case 0:
				tex = { 0,0 };
				break;
			case 1:
				tex = { 0,1 };
				break;
			case 2:
				tex = { 1,0 };
				break;
			case 3:
				tex = { 1,1 };
				break;
			}

			vertex.push_back(Vertex{ vertecies.GetVertecies()[i], tex });
		}

		cameraBuff = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 3);

		AddBindable(cameraBuff);

		Pointlight = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 5 * 4, 1);

		AddBindable(Pointlight);

		std::vector<DirectX::XMFLOAT4> data;

		data.push_back({ 1.0f, 0.5f, 0.31f,0 });
		data.push_back({ 1.0f, 0.5f, 0.31f, 0 });
		data.push_back({ 0.5f, 0.5f, 0.5f, 32 });

		AddBindable(Atlas::PixelConstantBuffer::Create(data.data(), sizeof(DirectX::XMFLOAT4) * data.size(), 2));

		auto vs = Atlas::VertexShader::Create("BrickWallVS.cso");
		auto blob = vs->GetBlob();
		AddBindable(std::move(vs));
		AddBindable(Atlas::PixelShader::Create("BrickWallPS.cso"));
		std::vector<Atlas::InputElement> elemnts;
		elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
		elemnts.push_back({ "TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT });
		AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "PlaneLayout"));

		AddBindable(Atlas::Texture::Create(R"(C:\Users\Dario\Downloads\brickwall.jpg)", false, 0));
		AddBindable(Atlas::Sampler::Create(false, false, 0, 0));

		AddBindable(Atlas::Texture::Create(R"(C:\Users\Dario\Downloads\brickwall_normal.jpg)", false, 1));
		AddBindable(Atlas::Sampler::Create(false, false, 0, 1));

		AddBindable(Atlas::VertexBuffer::Create(vertex.data(), vertex.size() * sizeof(Vertex), sizeof(Vertex), "Plane"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Plane"));
		AddBindable(Atlas::TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 0.7f, 1, 100000)));

		pos = { (float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10) };
		auto model = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
		AddBindable(Atlas::VertexConstantBuffer::Create(&model, sizeof(model), 1));

		AddBindable(Atlas::Topology::Create());
	}

	DirectX::XMMATRIX GetTransformXM() override
	{
		std::vector<DirectX::XMFLOAT4> data;

		auto front = camera->GetFront().m128_f32;

		data.push_back({ camera->GetPosition().x, camera->GetPosition().y,camera->GetPosition().z, 0 });
		data.push_back({ front[0], front[1], front[2], 0 });
		data.push_back({ cos(DirectX::XMConvertToRadians(0.01f)), cos(DirectX::XMConvertToRadians(2)), 0, 0 });

		cameraBuff->ImmidiateUpdate(data.data(), sizeof(DirectX::XMFLOAT4) * data.size());

		data.clear();
		for (int i = 0; i < 4; i++)
		{
			data.push_back({ lightPos[i].x, lightPos[i].y, lightPos[i].z, 0 });
			data.push_back({ 1.0f,	0.09f,	0.032f ,0 });
			data.push_back({ 0.2f, 0.2f, 0.2f, 0 });
			data.push_back({ 0.5f, 0.5f, 0.5f, 0 });
			data.push_back({ 1.0f, 1.0f, 1.0f, 0 });
		}
		Pointlight->ImmidiateUpdate(data.data(), sizeof(DirectX::XMFLOAT4) * data.size());

		return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * camera->GetTransform();
	}

private:
	Atlas::Camera* camera;
	std::shared_ptr<Atlas::PixelConstantBuffer> cameraBuff;
	std::shared_ptr<Atlas::PixelConstantBuffer> Pointlight;
	DirectX::XMFLOAT3* lightPos;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3* dir;
};