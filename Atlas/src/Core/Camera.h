#pragma once
#include "Window/GUI.h"
#include "Core/TimeStep.h"
#include <DirectXMath.h>

namespace Atlas
{
	//The camera controls class
	//It creates a view matrix that can be moved by right clicking
	//And pressing the WASDQE keys for movement and moving the mouse for rotation
	class Camera
	{
	public:
		//Constructor
		//Takes the max and min values the position slider will go to
		//And your speed in the world
		Camera(float maxPos, float minPos, float speed);
		~Camera();

		//Updates the camera values
		void Update(TimeStep timeStep);
		DirectX::XMFLOAT3 GetPosition() { return DirectX::XMFLOAT3{ m_Position[0], m_Position[1], m_Position[2] }; }
		DirectX::XMFLOAT3 GetRotation() { return DirectX::XMFLOAT3{ m_Rotation[0], m_Rotation[1], m_Rotation[2] }; }
		DirectX::XMVECTOR GetFront() { return DirectX::XMVector3Transform(DirectX::g_XMIdentityR2, DirectX::XMMatrixRotationRollPitchYaw(m_Rotation[1], m_Rotation[0], 0.0f));	}

		void Flip() { m_Rotation[0] = -m_Rotation[0]; m_Rotation[1] = -m_Rotation[1]; m_Rotation[2] = -m_Rotation[2]; CalculateViewMatrix(); }

		//Gets the transform
		DirectX::XMMATRIX GetTransform() { return m_ViewMatrix; };
	private:
		//A private function that translate the position of the camera
		//With the direction it is looking at
		void Translate(DirectX::XMFLOAT3 translation);

		void CalculateViewMatrix();

		GUI m_GUI;	//The gui that will contain all of the sliders for the camera
		float* m_Position;	//The the position of the camera
		float* m_Rotation;	//The the rotation of the camera
		float m_Speed;	//The speed of the camera
		std::tuple<int, int> m_InitialPosition;	//The position of the camera
		bool m_ResetInitialPosition;	//Whether or not the the initial position should be reset

		DirectX::XMMATRIX m_ViewMatrix;		//The view matrix
	};
}