#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class DX11Renderer;

class Grid
{
public:
	Grid(DX11Renderer* renderer, int size = 100, float step = 1.0f);
	
	void Draw();

	struct GridVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

private:
	DX11Renderer* m_renderer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_vertexCount;
};