#include "Header1.h"

static Atlas::Camera* camera;

#include "Header.h"
typedef unsigned long long ull;

class Test : public Atlas::Layer
{
public:
	Test()
	{
	}

	void OnAttach() override 
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

class TestLayer : public Atlas::Layer
{
public:
	TestLayer(std::string name)
		: Layer(name) {}

	void OnUpdate(Atlas::TimeStep t) override
	{
		Sleep(10);
	}

	//void OnEvent(Atlas::Event& e) override
	//{
	//	auto test = Atlas::EventDispatcher(e);
	//	test.Dispatch<Atlas::KeyPressedEvent>(std::bind(&TestLayer::OnPress, this, std::placeholders::_1));
	//	test.Dispatch<Atlas::MouseMovedEvent>(std::bind(&TestLayer::OnMove, this, std::placeholders::_1));
	//	test.Dispatch<Atlas::WindowResizeEvent>(std::bind(&TestLayer::OnResize, this, std::placeholders::_1));
	//}
	//
	//void OnMove(Atlas::MouseMovedEvent& e)
	//{
	//	std::cout << e.ToString() << std::endl;
	//	auto [x, y] = Atlas::Input::GetCursorPosition();
	//	std::cout << x << "  " << y << std::endl;
	//}
	//
	//void OnPress(Atlas::KeyPressedEvent& e)
	//{
	//	std::cout << e.ToString() << std::endl;
	//	std::cout << Atlas::Input::IsKeyPressed(e.GetKeyCode()) << std::endl;
	//}
	//
	//void OnResize(Atlas::WindowResizeEvent& e)
	//{
	//	std::cout << e.ToString() << std::endl;
	//	auto [x, y] = Atlas::Input::GetWindowSize();
	//	std::cout << x << "  " << y << std::endl;
	//}
};

class TestApp : public Atlas::Application
{
public:
	TestApp()
		: Atlas::Application("Test", 1024, 700)
	{
		PushLayer(std::shared_ptr<TestLayer>(new TestLayer("Layer1")));
		//PushLayer(new Test2());
	}

private:
};

int main()
{
	TestApp app;
	app.Run();

	//std::cout << "OnAttach" << std::endl;
	//std::cout << "OnUpdate" << std::endl;
	//std::cout << "OnDetach" << std::endl;
	//std::cin.get();
}