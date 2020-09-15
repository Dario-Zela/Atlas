#pragma once
#include <DirectXMath.h>
namespace dx = DirectX;

namespace Atlas
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		inline void SetPosition(const dx::XMFLOAT3& position) { m_Position = position; RecalculateViewMatrix();}
		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		
		inline const dx::XMFLOAT3& GetPosition() const { return m_Position; }
		inline const float& GetRotation() const { return m_Rotation; }

		inline const dx::XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const dx::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
		inline const dx::XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();

		dx::XMMATRIX m_ProjectionMatrix;
		dx::XMMATRIX m_ViewMatrix;
		dx::XMMATRIX m_ViewProjectionMatrix;

		dx::XMFLOAT3 m_Position;
		float m_Rotation = 0.0f;
	};
}