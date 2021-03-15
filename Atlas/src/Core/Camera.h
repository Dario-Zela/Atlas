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

		//Gets the position of the camera
		DirectX::XMFLOAT3 GetPosition();
		//Gets the rotation of the camera
		DirectX::XMFLOAT3 GetRotation();
		//Gets a vector parallel to the direction 
		//the camera is pointing
		DirectX::XMVECTOR GetFront();

		//Flips the camera
		void Flip();

		//Gets the transform
		DirectX::XMMATRIX GetTransform();
	private:
		//A private function that translate the position of the camera
		//With the direction it is looking at
		void Translate(DirectX::XMFLOAT3 translation);

		void CalculateViewMatrix();

		GUI m_GUI;	//The GUI that will contain all of the sliders for the camera
		float* m_Position;	//The position of the camera
		float* m_Rotation;	//The rotation of the camera
		float m_Speed;	//The speed of the camera
		std::tuple<int, int> m_InitialPosition;	//The position of the camera
		bool m_ResetInitialPosition;	//Whether or not the initial position should be reset

		DirectX::XMMATRIX m_ViewMatrix;		//The view matrix
	};
}