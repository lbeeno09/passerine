#include "Camera.h"

#include <algorithm>
#include <cmath>

namespace Engine
{
	Camera::Camera() : m_yaw(0.0f), m_pitch(0.0f)
	{
		m_position = { 0.0f, 0.0f, -5.0f };
		//m_target = { 0.0f, 0.0f, 0.0f };
		//m_up = { 0.0f, 1.0f, 0.0f };

		Update();
	}

	void Camera::Update()
	{
		float r = cosf(m_pitch);
		float x = r * sinf(m_yaw);
		float y = sinf(m_pitch);
		float z = r * cos(m_yaw);

		DirectX::XMVECTOR posVec = DirectX::XMLoadFloat3(&m_position);
		DirectX::XMVECTOR forwardVec = DirectX::XMVectorSet(x, y, z, 0.0f);

		DirectX::XMVECTOR globalUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR rightVec = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(globalUp, forwardVec));

		DirectX::XMVECTOR localUp = DirectX::XMVector3Cross(forwardVec, rightVec);

		m_viewMatrix = DirectX::XMMatrixLookToLH(posVec, forwardVec, localUp);
	}

	void Camera::Rotate(float dx, float dy)
	{
		m_yaw += dx;
		m_pitch -= dy;

		m_pitch = std::clamp(m_pitch, -DirectX::XM_PIDIV2 + 0.01f, DirectX::XM_PIDIV2 - 0.01f);
	}

	void Camera::MoveForward(float dist)
	{
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
		pos = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(dist), GetForwardVector(), pos);

		DirectX::XMStoreFloat3(&m_position, pos);
	}

	void Camera::MoveRight(float dist)
	{
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
		pos = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(dist), GetRightVector(), pos);

		DirectX::XMStoreFloat3(&m_position, pos);
	}

	void Camera::MoveUp(float dist)
	{
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
		pos = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(dist), GetUpVector(), pos);

		DirectX::XMStoreFloat3(&m_position, pos);
	}

	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return m_viewMatrix;
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix(float aspectRatio) const
	{
		return DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(45.0f),
			aspectRatio,
			0.1f, 1000.0f
		);
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_position = { x, y, z };
	}

	DirectX::XMVECTOR Camera::GetForwardVector() const
	{
		float r = cosf(m_pitch);
		DirectX::XMVECTOR f = DirectX::XMVectorSet(
			r * sinf(m_yaw),
			sinf(m_pitch),
			r * cosf(m_yaw),
			0.0f
		);

		return DirectX::XMVector3Normalize(f);
	}

	DirectX::XMVECTOR Camera::GetRightVector() const
	{
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, GetForwardVector()));
	}

	DirectX::XMVECTOR Camera::GetUpVector() const
	{
		return DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}
}