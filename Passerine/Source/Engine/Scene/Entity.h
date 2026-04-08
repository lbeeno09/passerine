#pragma once

#include <DirectXMath.h>
#include <string>

namespace Engine
{
	class Entity
	{
	public:
		Entity(const std::string& name);
		virtual ~Entity() = default;
		
		virtual void Draw() = 0;

		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }

		DirectX::XMFLOAT3& GetPosition() { return m_position; }
		DirectX::XMFLOAT3& GetRotation() { return m_rotation; }
		DirectX::XMFLOAT3& GetScale() { return m_scale; }

		DirectX::XMMATRIX GetWorldMatrix() const;

	protected:
		std::string m_name;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
	};
}