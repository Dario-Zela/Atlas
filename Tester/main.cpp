#include "Header.h"
/*
class Test : public Atlas::Layer
{
public:
	Test()
		: Atlas::Layer("Test"), scale(1), model(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\resources\objects\nanosuit\nanosuit.obj)"), depth(1024, 700)
	{	
		camera = new Atlas::Camera(1000, 1, 0.1f);
		settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 100000);
		settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::COLOR_DIFFUSE | Atlas::MeshProprietiesFlags::SHININESS);
		settings.textureFlags = (uint)Atlas::MeshTextureFlags::DIFFUSE;
		settings.addAnisotropicFiltering = true;
		settings.addMipMapping = true;
		settings.maxAnisotropy = 7;
		//settings.addBlending = true;
		model.ApplyTransform("Visor", *&(DirectX::XMMatrixTranslation(0,0,10) * DirectX::XMMatrixScaling(scale, scale, scale)));
		Atlas::Graphics::BindDefaultViewPort();

		objRot = new float[3]();
		gui.Init();
		gui.AddSliderFloat3("Rot", objRot, -DirectX::XM_2PI, DirectX::XM_2PI, 0.1f);
		gui.AddSliderFloat("Scale", &scale, 0, 10, 0.01f);
		Atlas::Graphics::GetRenderTarget()->Bind(depth.GetDepthStencilBuffer().Get());
	}

	void OnUpdate(Atlas::TimeStep time) override
	{
		camera->Update();

		Atlas::Graphics::GetRenderTarget()->Clear(0, 0, 1);
		depth.Clear();
		DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * camera->GetTransform();
		model.ApplyTransform("Visor", *&(DirectX::XMMatrixTranslation(0, 0, 1) * DirectX::XMMatrixScaling(scale, scale, scale)));
		model.Draw(settings, trans);
	}

private:
	Atlas::GUI gui;
	float* objRot;
	float scale;
	Atlas::ModelDrawSettings settings;
	Atlas::Scene model;
	Atlas::DepthStencilBuffer depth;
};

class Test2 : public Atlas::Layer
{
public:
	Test2()
		: Atlas::Layer("Test"), scale(1), model(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\sponza\glTF\Sponza.gltf)")
	{
		settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 100000);
		settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::COLOR_DIFFUSE | Atlas::MeshProprietiesFlags::SHININESS);
		settings.textureFlags = (uint)Atlas::MeshTextureFlags::DIFFUSE;
		settings.addAnisotropicFiltering = true;
		settings.addMipMapping = true;
		settings.maxAnisotropy = 7;
		//settings.addBlending = true;
		Atlas::Graphics::BindDefaultViewPort();

		objRot = new float[3]();
		gui.Init();
		gui.AddSliderFloat3("Rot", objRot, -DirectX::XM_2PI, DirectX::XM_2PI, 0.1f);
		gui.AddSliderFloat("Scale", &scale, 0, 10, 0.01f);
	}

	void OnUpdate(Atlas::TimeStep time) override
	{
		camera->Update();

		DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * camera->GetTransform();
		model.Draw(settings, trans);
		Atlas::Graphics::EndFrame(1);
	}

private:
	float* objRot;
	float scale;
	Atlas::GUI gui;
	Atlas::ModelDrawSettings settings;
	Atlas::Scene model;
};
*/

class Test : public Atlas::Layer
{
public:
	Test()
		:m_Scene(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\sponza\glTF\Sponza.gltf)"), m_Camera(1000, -1000, 0.1f)
	{
		m_Settings.addMipMapping = true;
		m_Settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::COLOR_DIFFUSE | Atlas::MeshProprietiesFlags::SHININESS);
		m_Settings.textureFlags = (uint)Atlas::MeshTextureFlags::DIFFUSE;
		m_Settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 1240/700, 1, 100000);

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
			step.AddBindable(Atlas::Topology::Create());
			step.AddBindable(Atlas::Sampler::Create(true, true, 7, 0));
			Atlas::Graphics::BindDefaultViewPort();
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
		m_Camera.Update();
		DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * m_Camera.GetTransform();
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
	Atlas::Camera m_Camera;
	LambertianRenderGraph rg;
};

class TestApp : public Atlas::Application
{
public:
	TestApp()
		: Atlas::Application("Test", 1024, 700)
	{
		PushLayer(new Test());
	}

private:
};

int main()
{
	TestApp app;
	app.Run();
}