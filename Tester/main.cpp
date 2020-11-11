#include "Atlas.h"

class Test : public Atlas::Layer
{
public:
	Test()
		: Atlas::Layer("Test"), camera(1000, -1000, 0.1f), model(R"(C:\Users\Dario\Desktop\Dario\Atlas\Tester\assets\Models\sponza\glTF\Sponza.gltf)")
	{	
		settings.pixelShaderPath = "TestPixel.cso";
		settings.vertexShaderPath = "TestVertex.cso";
		settings.viewMatrix = DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 100000);
		settings.proprietiesFlags = (uint)(Atlas::MeshProprietiesFlags::TEXTURE_COORDINATES | Atlas::MeshProprietiesFlags::COLOR_DIFFUSE | Atlas::MeshProprietiesFlags::SHININESS);
		settings.textureFlags = (uint)Atlas::MeshTextureFlags::DIFFUSE;
		settings.addAnisotropicFiltering = true;
		settings.addMipMapping = true;
		settings.maxAnisotropy = 7;
		settings.addBlending = true;
		Atlas::Graphics::BindDefaultViewPort();

		objRot = new float[3]();
		gui.Init();
		gui.AddSliderFloat3("Rot", objRot, -DirectX::XM_2PI, DirectX::XM_2PI, 0.1f);
	}

	void OnUpdate(Atlas::TimeStep time) override
	{
		camera.Update();

		Atlas::Graphics::ClearScreen(0, 0, 1);
		DirectX::XMMATRIX trans = DirectX::XMMatrixRotationRollPitchYaw(objRot[0], objRot[1], objRot[2]) * camera.GetTransform();
		model.Draw(settings, trans);

		Atlas::Graphics::EndFrame(1);
	}

private:
	Atlas::Camera camera;
	Atlas::GUI gui;
	float* objRot;
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

	}

private:
};

int main()
{
	TestApp app;
	app.Run();
}