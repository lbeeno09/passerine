#include "Grid.h"

#include "Renderer/DX11Renderer.h"

#include <vector>

Grid::Grid(DX11Renderer* renderer, int size, float step) : m_renderer(renderer)
{
	std::vector<GridVertex> vertices;
	float halfSize = (size * step) / 2.0f;

	DirectX::XMFLOAT4 gridColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	DirectX::XMFLOAT4 axisColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	for(int i = 0; i <= size; i++)
	{
		float pos = -halfSize + (i * step);
		DirectX::XMFLOAT4 currentSetColor = (i == size / 2) ? axisColor : gridColor;

		vertices.push_back({ {pos, 0.0f, -halfSize}, currentSetColor });
		vertices.push_back({ {pos, 0.0f,  halfSize}, currentSetColor });

		vertices.push_back({ {-halfSize, 0.0f, pos}, currentSetColor });
		vertices.push_back({ { halfSize, 0.0f, pos}, currentSetColor });
	}
	m_vertexCount = static_cast<UINT>(vertices.size());

	// Create Vertex Buffer
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(GridVertex) * m_vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = vertices.data();

	m_renderer->GetDevice()->CreateBuffer(&vbd, &vInitData, m_vertexBuffer.GetAddressOf());
}

void Grid::Draw()
{
	ID3D11DeviceContext* context = m_renderer->GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->Draw(m_vertexCount, 0);

}
