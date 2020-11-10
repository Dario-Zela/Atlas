#pragma once
#include "Window/GUI.h"

namespace DirectX
{
	struct XMMATRIX;
}

namespace Atlas
{
	class Camera
	{
	public:
		Camera(float maxPos, float minPos, float scale);
		~Camera();

		void Update();
		DirectX::XMMATRIX GetTransform();
	private:
		GUI m_GUI;
		float* m_Position;
		float* m_Rotation;
		float m_Scale;
	};
}