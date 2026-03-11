#include "Mesh.h"

#include "Renderer/DX11Renderer.h"

Mesh::Mesh(DX11Renderer* renderer) : m_renderer(renderer), m_indexCount(0)
{

}

Mesh::~Mesh() = default;

void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_indexCount = (UINT)indices.size();
	auto device = m_renderer->GetDevice();

	// Create Vertex Buffer
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * (UINT)vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = vertices.data();
	device->CreateBuffer(&vbd, &vInitData, &m_vertexBuffer);

	// Create Index Buffer
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * m_indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices.data();
	device->CreateBuffer(&ibd, &iInitData, &m_indexBuffer);
}

void Mesh::CreateCube()
{
	std::vector<Vertex> vertices = {
		{ {-0.5f, -0.5f, -0.5f }, {1.0f, 1.0f, 1.0f, 1.0f} },
		{ { 0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ { 0.5f,  0.5f, -0.5f }, {0.0f, 1.0f, 0.0f, 1.0f} },
		{ {-0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 1.0f, 1.0f} },
		{ {-0.5f, -0.5f,  0.5f }, {1.0f, 1.0f, 0.0f, 1.0f} },
		{ { 0.5f, -0.5f,  0.5f }, {1.0f, 0.0f, 1.0f, 1.0f} },
		{ { 0.5f,  0.5f,  0.5f }, {0.0f, 1.0f, 1.0f, 1.0f} },
		{ {-0.5f,  0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f} }
	};

	std::vector<unsigned int> indices = {
		0, 2, 1,  0, 3, 2, // Front
		1, 6, 5,  1, 2, 6, // Right
		5, 7, 4,  5, 6, 7, // Back
		4, 3, 0,  4, 7, 3, // Left
		4, 5, 1,  4, 1, 0, // Bottom
		3, 6, 7,  3, 2, 6  // Top
	};

	Initialize(vertices, indices);
}

void Mesh::Draw()
{
	ID3D11DeviceContext* context = m_renderer->GetContext();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(m_indexCount, 0, 0);
}
