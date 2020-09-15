#include "pch.h"
#include "OrthographicCamera.h"

namespace Atlas
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(dx::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(), m_Position(1.0f, 1.0f, 1.0f)
	{
		m_ViewMatrix = dx::XMMatrixIdentity();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = dx::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		dx::XMMATRIX transform = dx::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z) *
			dx::XMMatrixRotationZ(dx::XMConvertToRadians(m_Rotation));

		m_ViewMatrix = dx::XMMatrixInverse(nullptr, transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}