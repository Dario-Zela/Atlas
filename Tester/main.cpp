#include "Atlas.h"

static Atlas::Camera* camera;

class Test : public Atlas::Layer
{
public:
	Test()
		: Atlas::Layer("Test"), scale(1), model(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\resources\objects\nanosuit\nanosuit.obj)"), depth(1024, 700)
	{	
		camera = new Atlas::Camera(1000, 1, 0.1f);
		settings.pixelShaderPath = "TestPixel.cso";
		settings.vertexShaderPath = "TestVertex.cso";
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
		settings.pixelShaderPath = "TestPixel.cso";
		settings.vertexShaderPath = "TestVertex.cso";
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

class TestApp : public Atlas::Application
{
public:
	TestApp()
		: Atlas::Application("Test", 1024, 700)
	{
		PushLayer(new Test());
		PushLayer(new Test2());
	}

private:
};

int main()
{
	TestApp app;
	app.Run();
}