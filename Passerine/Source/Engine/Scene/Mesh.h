#pragma once

#include "Engine/Scene/Entity.h"
#include "Engine/Renderer//DX11Renderer.h"
#include "Engine/Core/GraphicsTypes.h"

#include <wrl/client.h>
#include <vector>


namespace Engine
{
	class Mesh : public Entity
	{
	public:
		Mesh(DX11Renderer* renderer, const std::string& name = "Cube");
		~Mesh() = default;

		void Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void CreateCube();
		void CreatePlane();
		void CreateSphere();
		void CreateCyllinder();
		void CreateCapsule();
		void CreateCone();
		void Draw() override;

	private:
		DX11Renderer* m_renderer;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_indexCount = 0;
	};
}