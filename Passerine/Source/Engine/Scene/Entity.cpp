#include "Entity.h"

#include "Engine/Renderer/DX11Renderer.h"

namespace Engine
{
	Entity::Entity(const std::string& name) : m_name(name)
	{
		m_position = { 0.0f, 0.0f, 0.0f };
		m_rotation = { 0.0f, 0.0f, 0.0f };
		m_scale = { 1.0f, 1.0f, 1.0f };
	}

	DirectX::XMMATRIX Entity::GetWorldMatrix() const
	{
		return DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
			DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	}
}