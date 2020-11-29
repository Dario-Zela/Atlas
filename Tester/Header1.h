#pragma once
#include "Atlas.h"

class Cube : public Atlas::Drawable
{
public:
	Cube(int x, int y, Atlas::Camera* camera)
		: camera(camera), x(x * 2), y(y * 2)
	{
		auto vertecies = Atlas::Cube::Make();
		vertecies.Transform(DirectX::XMMatrixScaling(10000000, 10000000, 10000000));
		AddBindable(Atlas::VertexBuffer::Create(vertecies.GetVertecies().data(), vertecies.GetVertecies().size() * sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), "Cube"));
		AddBindable(Atlas::IndexBuffer::Create(vertecies.GetIndecies().data(), vertecies.GetIndecies().size() * sizeof(unsigned short), "Cube"));
		AddBindable(Atlas::TransformationConstantBuffer::Create(*this, DirectX::XMMatrixPerspectiveLH(1, 1240 / 700, 1, 100000)));
		AddBindable(Atlas::Topology::Create());

	}

	DirectX::XMMATRIX GetTransformXM() override
	{
		return DirectX::XMMatrixTranslation(x, y, 0) * camera->GetTransform();
	}

private:
	Atlas::Camera* camera;
	int x, y;
};