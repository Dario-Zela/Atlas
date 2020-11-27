#include "pch.h"
#include "Camera.h"
#include "Core/Input.h"

namespace Atlas
{
	Camera::Camera(float maxPos, float minPos, float speed)
	{
		//Initialise the GUI
		m_GUI.Init("Camera");

		//Initialise the position, rotation and speed
		m_Position = new float[3]();
		m_Rotation = new float[2]();
		m_Speed = speed;

		//Set the sliders
		m_GUI.AddSliderFloat3("CameraPosition", m_Position, minPos, maxPos, speed);
		m_GUI.AddSliderFloat2("CameraRotation", m_Rotation, -DirectX::XM_2PI, DirectX::XM_2PI, 0.03f);

		//Indicate that the position should be reset whenever possible
		m_ResetInitialPosition = true;
	}

	Camera::~Camera()
	{
		//Delete the GUI
		m_GUI.m_Alive = false;
		//Delete the rotation and position data
		delete[] m_Position;
		delete[] m_Rotation;
	}

	void Camera::Update()
	{
		//If the user presses the Right mouse button
		if (Input::IsMouseButtonPressed(MK_RBUTTON)) 
		{
			//Set the initial position if it has an old or unset value
			if (m_ResetInitialPosition)
			{
				m_InitialPosition = Input::GetCursorPosition();
				m_ResetInitialPosition = false;
			}

			//Then if the positions differ
			if (Input::GetCursorPosition() != m_InitialPosition)
			{
				//Get the differences between the positions and add them to the rotaion
				std::tuple<int, int> currentPosition = Input::GetCursorPosition();

				m_Rotation[0] += (std::get<0>(currentPosition) - std::get<0>(m_InitialPosition)) * 0.01f;
				m_Rotation[1] = std::clamp(m_Rotation[1] + (std::get<1>(currentPosition) - std::get<1>(m_InitialPosition)) * 0.01f,
					-DirectX::XM_PIDIV2, DirectX::XM_PIDIV2);	//This is clamped to stop users from rotating 360 degrees

				//Update the initial position
				m_InitialPosition = currentPosition;
			}

			//This translates the position depending on which key has been pressed
			if (Input::IsKeyPressed('W'))
			{
				Translate({ 0,0,1 });
			}
			else if (Input::IsKeyPressed('S'))
			{
				Translate({ 0,0,-1 });
			}
			if (Input::IsKeyPressed('A'))
			{
				Translate({ -1,0,0 });
			}
			else if (Input::IsKeyPressed('D'))
			{
				Translate({ 1,0,0 });
			}
			if (Input::IsKeyPressed('Q'))
			{
				Translate({ 0,1,0 });
			}
			else if (Input::IsKeyPressed('E'))
			{
				Translate({ 0,-1,0 });
			}

		}
		//If the user doesn't press the right mouse button
		//It is likely that the mouse position will have become 
		//Old and requires to be updated
		else
		{
			if (!m_ResetInitialPosition)
			{
				m_ResetInitialPosition = true;
			}
		}
		//The gui is then updated
		m_GUI.Broadcast();
	}

	DirectX::XMMATRIX Camera::GetTransform()
	{
		//The rotations are applied to the vector that is forward from us
		DirectX::XMVECTOR lookVector = DirectX::XMVector3Transform(DirectX::g_XMIdentityR2,
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation[1], m_Rotation[0], 0.0f)
		);

		//The view matrix is created by setting two position, one being the current position
		//And one being one look vector away from us. 
		DirectX::XMVECTOR cameraPosition = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(m_Position));
		DirectX::XMVECTOR cameraTarget = DirectX::XMVectorAdd(cameraPosition, lookVector);
		return DirectX::XMMatrixLookAtLH(cameraPosition, cameraTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Camera::Translate(DirectX::XMFLOAT3 translation)
	{
		//For a translation, the original movement is rotated and scaled by the speed
		DirectX::XMVECTOR rotatedTranslation = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&translation),
			DirectX::XMMatrixRotationRollPitchYaw(m_Rotation[1], m_Rotation[0], 0.0f) * DirectX::XMMatrixScaling(m_Speed, m_Speed, m_Speed));

		//It is then added to the current positions
		m_Position[0] += rotatedTranslation.m128_f32[0];
		m_Position[1] += rotatedTranslation.m128_f32[1];
		m_Position[2] += rotatedTranslation.m128_f32[2];
	}
}