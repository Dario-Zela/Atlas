#include "Atlas.h"

class Lay : public Atlas::Layer
{
public:
	Lay()
		:camera(0, 1200, 0, 700)
	{

	}

	// Inherited via Layer
	virtual void OnAttach() override
	{
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate(Atlas::TimeStep time) override
	{
		Atlas::Graphics::ClearScreen(0, 0, 0);
		Atlas::ViewPort vp(0, 1200, 0, 700, 0, 1);
		Atlas::Renderer2D::BeginScene(camera, vp);
		Atlas::Renderer2D::DrawQuad(dx::XMFLOAT2(100, 200), dx::XMFLOAT2(100, 200), dx::XMVectorReplicate(1));
		Atlas::Renderer2D::EndScene();
	}

	virtual void OnEvent(Atlas::Event& e) override
	{
	}

private:
	Atlas::OrthographicCamera camera;
};

class App : public Atlas::Application
{
public:
	App() : Atlas::Application("Test", 1024, 700)
	{
		PushLayer(new Lay());
	}
};

int main()
{
	App app;
	app.Run();
}