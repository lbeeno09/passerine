#pragma once

#include "Graphics/GraphicsTypes.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class DX11Renderer;

class Mesh
{
public:
	Mesh(DX11Renderer* renderer);
	~Mesh();

	void Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void CreateCube();
	void Draw();

private:
	DX11Renderer* m_renderer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount;
};
