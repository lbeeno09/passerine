#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera();

	void Update();

	void Rotate(float dx, float dy);
	void MoveForward(float dist);
	void MoveRight(float dist);
	void MoveUp(float dist);

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix(float aspectRatio) const;

	void SetPosition(float x, float y, float z);

private:
	DirectX::XMVECTOR GetForwardVector() const;
	DirectX::XMVECTOR GetRightVector() const;
	DirectX::XMVECTOR GetUpVector() const;

	DirectX::XMFLOAT3 m_position;
	float m_yaw;
	float m_pitch;
	//DirectX::XMFLOAT3 m_target;
	//DirectX::XMFLOAT3 m_up;

	DirectX::XMMATRIX m_viewMatrix;

};
