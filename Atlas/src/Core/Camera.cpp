#include "pch.h"
#include "Camera.h"
#include "Core/Input.h"
#include <DirectXMath.h>

namespace Atlas
{
	Camera::Camera(float maxPos, float minPos, float scale)
	{
		m_GUI.Init("Camera");

		m_Position = new float[3]();
		m_Rotation = new float[3]();

		m_Scale = scale;

		m_GUI.AddSliderFloat3("CameraPosition", m_Position, minPos, maxPos, scale);
		m_GUI.AddSliderFloat3("CameraRotation", m_Rotation, -DirectX::XM_2PI, DirectX::XM_2PI, 0.03f);
	}

	Camera::~Camera()
	{
		m_GUI.m_Alive = false;
		delete[] m_Position;
		delete[] m_Rotation;
	}

	void Camera::Update()
	{
		if (Input::IsKeyPressed(VK_LEFT))
		{
			m_Position[0] -= m_Scale;
		}
		else if (Input::IsKeyPressed(VK_RIGHT))
		{
			m_Position[0] += m_Scale;
		}

		if (Input::IsKeyPressed(VK_UP))
		{
			m_Position[1] += m_Scale;
		}
		else if (Input::IsKeyPressed(VK_DOWN))
		{
			m_Position[1] -= m_Scale;
		}
		
		m_Position[2] += Input::GetVScrool() == 0 ? 0 : Input::GetVScrool() < 0 ? m_Scale * -1 : m_Scale;

		if (Input::IsKeyPressed('Q'))
		{
			if (Input::IsKeyPressed(VK_SHIFT))
			{
				m_Rotation[0] -= 0.03f;
			}
			else
			{
				m_Rotation[0] += 0.03f;
			}
		}
		if (Input::IsKeyPressed('E'))
		{
			if (Input::IsKeyPressed(VK_SHIFT))
			{
				m_Rotation[1] -= 0.03f;
			}
			else
			{
				m_Rotation[1] += 0.03f;
			}
		}
		if (Input::IsKeyPressed('R'))
		{
			if (Input::IsKeyPressed(VK_SHIFT))
			{
				m_Rotation[2] -= 0.03f;
			}
			else
			{
				m_Rotation[2] += 0.03f;
			}
		}

		m_GUI.Broadcast();
	}

	DirectX::XMMATRIX Camera::GetTransform()
	{
		return DirectX::XMMatrixTranslation(m_Position[0], m_Position[1], m_Position[2]) * 
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation[0], m_Rotation[1], m_Rotation[2]);
	}
}