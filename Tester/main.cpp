#include "Header1.h"

static Atlas::Camera* camera;

#include "Header.h"
typedef unsigned long long ull;

class Test : public Atlas::Layer
{
public:
	Test()
	{
		camera = new Atlas::Camera(1000, -1000, 10);

		lightPos = new DirectX::XMFLOAT3[4]();

		dir = new DirectX::XMFLOAT3();

		att = new DirectX::XMFLOAT3();

		settings.addMipMapping = true;
		settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::MODEL);
		settings.textureFlags = (uint)(Atlas::MeshTextureFlags::DIFFUSE | Atlas::MeshTextureFlags::NORMALS | Atlas::MeshTextureFlags::SPECULAR);
		settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 0.7f, 1, 100000);

		gui.Init();
		gui.AddSliderFloat3("LightPos1", (float*)&lightPos[0], -10, 10, 0.1f);
		gui.AddSliderFloat3("LightPos2", (float*)&lightPos[1], -10, 10, 0.1f);
		gui.AddSliderFloat3("LightPos3", (float*)&lightPos[2], -10, 10, 0.1f);
		gui.AddSliderFloat3("LightPos4", (float*)&lightPos[3], -10, 10, 0.1f);
		gui.AddSliderFloat3("Attenuation", (float*)att, 0, 10, 0.1f);

		gui.AddCheckBox("Flip", &m_Flip);

		for (int i = 0; i < 10; i++)
			cube.push_back(std::make_unique<Cube>(camera, lightPos, dir));
		for (int i = 0; i < 4; i++)
			light.push_back(std::make_unique<Light>(&lightPos[i], camera));

		//Atlas::Technique tech("default");
		//{
		//	Atlas::Step step("LambertianPass");
		//	cameraBuff = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 3);
		//
		//	step.AddBindable(cameraBuff);
		//
		//	Pointlight = Atlas::PixelConstantBuffer::Create(sizeof(DirectX::XMFLOAT4) * 5 * 4, 1);
		//
		//	step.AddBindable(Pointlight);
		//
		//	std::vector<DirectX::XMFLOAT4> data;
		//
		//	data.push_back({ 1.0f, 0.5f, 0.31f,0 });
		//	data.push_back({ 1.0f, 0.5f, 0.31f, 0 });
		//	data.push_back({ 0.5f, 0.5f, 0.5f, 32 });
		//
		//	step.AddBindable(Atlas::PixelConstantBuffer::Create(data.data(), sizeof(DirectX::XMFLOAT4) * data.size(), 2));
		//
		//	auto vs = Atlas::VertexShader::Create("BrickWallVS.cso");
		//	auto blob = vs->GetBlob();
		//	step.AddBindable(std::move(vs));
		//	step.AddBindable(Atlas::PixelShader::Create("BrickWallPS.cso"));
		//	step.AddBindable(Atlas::InputLayout::Create({ {"POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT},
		//												  {"TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT }}, blob, "Mesh"));
		//
		//	step.AddBindable(Atlas::Sampler::Create(true, true, 7));
		//	step.AddBindable(Atlas::Topology::Create());
		//	step.AddBindable(Atlas::Blendable::Create(true, 0));
		//	step.AddBindable(Atlas::Graphics::GetDefaultViewPort());
		//	tech.AddStep(step);
		//}

		//scene.AddTechnique(tech);
		//scene.LinkTechniques(rg);

		for (int i = 0; i < 10; i++)
		{
			cube[i]->LinkTechniques(rg);
		}

		for (int i = 0; i < 4; i++)
		{
			light[i]->LinkTechniques(rg);
		}
		screen.LinkTechniques(rg);

		for (int i = 0; i < 10; i++)
			cube[i]->Submit();
		for (int i = 0; i < 4; i++)
			light[i]->Submit();

		screen.Submit();
	}

	void OnUpdate(Atlas::TimeStep ts) override
	{
		camera->Update(ts);

		if (m_Flip != m_FlipPre)
		{
			camera->Flip();
			m_FlipPre = m_Flip;
		}

		rg.Execute();

		Atlas::Graphics::EndFrame(1);
	}

private:
	DirectX::XMFLOAT3* lightPos;
	DirectX::XMFLOAT3* dir;
	DirectX::XMFLOAT3* att;
	std::vector<std::unique_ptr<Cube>> cube;
	Atlas::GUI gui;
	Screen screen;
	bool m_Flip;
	bool m_FlipPre;
	Atlas::ModelDrawSettings settings;
	std::vector<std::unique_ptr<Light>> light;
	LambertianRenderGraph rg;
	std::shared_ptr<Atlas::ConstantBuffer> cameraBuff;
	std::shared_ptr<Atlas::ConstantBuffer> Pointlight;

	int x = 0;
};

//Bugged, need to make the backbuffer || depthBuffer be the same between these calls
/*
class Test2 : public Atlas::Layer
{
public:
	Test2()
		:m_Scene(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\resources\objects\nanosuit\nanosuit.obj)")
	{
		m_Settings.addMipMapping = true;
		m_Settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::COLOR_DIFFUSE | Atlas::MeshProprietiesFlags::SHININESS);
		m_Settings.textureFlags = (uint)Atlas::MeshTextureFlags::DIFFUSE;
		m_Settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 1240 / 700, 1, 100000);

		Atlas::Technique tech("default");
		{
			Atlas::Step step("LambertianPass");
			auto vs = Atlas::VertexShader::Create("TestVertex.cso");
			auto blob = vs->GetBlob();
			step.AddBindable(std::move(vs));
			step.AddBindable(Atlas::PixelShader::Create("TestPixel.cso"));
			std::vector<Atlas::InputElement> elemnts;
			elemnts.push_back({ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT });
			elemnts.push_back({ "TEXTURE_COORDS", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT });
			step.AddBindable(Atlas::InputLayout::Create(std::move(elemnts), blob, "MeshLayout"));
			step.AddBindable(Atlas::Sampler::Create(true, true, 7, 0));
			step.AddBindable(Atlas::Graphics::GetDefaultViewPort());
			tech.AddStep(step);
		}

		m_Scene.AddTechnique(tech);
		m_Scene.LinkTechniques(rg);

		objRot = new float[3]();
		gui.Init();
		gui.AddSliderFloat3("Rot", objRot, -DirectX::XM_2PI, DirectX::XM_2PI, 0.1f);
	}

	void OnUpdate(Atlas::TimeStep ts) override
	{
		camera->Update();
		DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * camera->GetTransform();
		m_Scene.Draw(m_Settings, trans);
		rg.Execute();
		Atlas::Graphics::EndFrame(1);
		rg.Reset();
	}

private:
	Atlas::GUI gui;
	float* objRot;
	Atlas::Scene m_Scene;
	Atlas::ModelDrawSettings m_Settings;
	LambertianRenderGraph2 rg;
};
*/

class TestApp : public Atlas::Application
{
public:
	TestApp()
		: Atlas::Application("Test", 1024, 700)
	{
		InnerPushLayer(new Test());
		//PushLayer(new Test2());
	}

private:
};

int main()
{
	TestApp app;
	app.Run();
}