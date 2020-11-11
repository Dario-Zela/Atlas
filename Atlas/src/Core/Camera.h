#pragma once
#include "Window/GUI.h"
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
		void Update();

		//Gets the transform
		DirectX::XMMATRIX GetTransform();
	private:
		//A private function that translate the position of the camera
		//With the direction it is looking at
		void Translate(DirectX::XMFLOAT3 translation);

		GUI m_GUI;	//The gui that will contain all of the sliders for the camera
		float* m_Position;	//The the position of the camera
		float* m_Rotation;	//The the rotation of the camera
		float m_Speed;	//The speed of the camera
		std::tuple<int, int> m_InitialPosition;	//The position of the camera
		bool m_ResetInitialPosition;	//Whether or not the the initial position should be reset
	};
}