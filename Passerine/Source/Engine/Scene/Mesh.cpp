#include "Mesh.h"

#include "Engine/Renderer/DX11Renderer.h"

namespace Engine
{
	Mesh::Mesh(DX11Renderer* renderer, const std::string& name) : Entity(name)
	{
		m_renderer = renderer;
	}


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

	void Mesh::CreatePlane()
	{
		std::vector<Vertex> vertices = {
	{ {-5.0f, 0.0f, -5.0f }, {0.3f, 0.3f, 0.3f, 1.0f} },
	{ { 5.0f, 0.0f, -5.0f }, {0.3f, 0.3f, 0.3f, 1.0f} },
	{ { 5.0f, 0.0f,  5.0f }, {0.3f, 0.3f, 0.3f, 1.0f} },
	{ {-5.0f, 0.0f,  5.0f }, {0.3f, 0.3f, 0.3f, 1.0f} },
		};

		std::vector<unsigned int> indices = {
			0, 2, 1,  0, 3, 2, // Front
		};

		Initialize(vertices, indices);
	}

	void Mesh::CreateSphere()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const int latBands = 20;
		const int lonBands = 20;
		const float radius = 0.5f;

		for(int lat = 0; lat <= latBands; lat++)
		{
			float theta = lat * DirectX::XM_PI / latBands;
			for(int lon = 0; lon <= lonBands; lon++)
			{
				float phi = lon * 2 * DirectX::XM_PI / lonBands;

				float x = cosf(phi) * sinf(theta);
				float y = cosf(theta);
				float z = sinf(phi) * sinf(theta);

				vertices.push_back({ { x * radius, y * radius, z * radius }, {0.8f, 0.8f, 0.8f, 1.0f} });
			}
		}
		for(int lat = 0; lat < latBands; lat++)
		{
			for(int lon = 0; lon < lonBands; lon++)
			{
				unsigned int first = (lat * (lonBands + 1)) + lon;
				unsigned int second = first + lonBands + 1;

				indices.push_back(first);
				indices.push_back(first + 1);
				indices.push_back(second);

				indices.push_back(second);
				indices.push_back(first + 1);
				indices.push_back(second + 1);
			}
		}

		Initialize(vertices, indices);
	}
	
	void Mesh::CreateCyllinder()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const int segments = 24;
		const float radius = 0.5f;
		const float height = 1.0f;

		for(int i = 0; i <= segments; i++)
		{
			float theta = i * DirectX::XM_2PI / segments;

			float x = cosf(theta) * radius;
			float z = sinf(theta) * radius;

			vertices.push_back({ { x, -height / 2.0f, z}, {0.6f, 0.6f, 0.6f, 1.0f} });
			vertices.push_back({ { x,  height / 2.0f, z}, {0.6f, 0.6f, 0.6f, 1.0f} });
		}

		for(int i = 0; i < segments; i++)
		{
			unsigned int b0 = i * 2;
			unsigned int t0 = i * 2 + 1;
			unsigned int b1 = (i + 1) * 2;
			unsigned int t1 = (i + 1) * 2 + 1;

			indices.push_back(b0);
			indices.push_back(t0);
			indices.push_back(t1);

			indices.push_back(b0);
			indices.push_back(t1);
			indices.push_back(b1);
		}

		Initialize(vertices, indices);
	}
	
	void Mesh::CreateCapsule()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const int latBands = 20;
		const int lonBands = 20;
		const float radius = 0.5f;
		const float height = 1.0f;

		for(int lat = 0; lat <= latBands; lat++)
		{
			float theta = lat * DirectX::XM_PI / latBands;
			float yOffset = (lat < latBands / 2) ? height / 2.0f : -height / 2.0f;
			for(int lon = 0; lon <= lonBands; lon++)
			{
				float phi = lon * 2 * DirectX::XM_PI / lonBands;

				float x = cosf(phi) * sinf(theta);
				float y = cosf(theta);
				float z = sinf(phi) * sinf(theta);

				vertices.push_back({ { x * radius, (y * radius) + yOffset, z * radius }, {0.8f, 0.8f, 0.8f, 1.0f} });
			}
		}
		for(int lat = 0; lat < latBands; lat++)
		{
			for(int lon = 0; lon < lonBands; lon++)
			{
				unsigned int first = (lat * (lonBands + 1)) + lon;
				unsigned int second = first + lonBands + 1;

				indices.push_back(first);
				indices.push_back(first + 1);
				indices.push_back(second);

				indices.push_back(second);
				indices.push_back(first + 1);
				indices.push_back(second + 1);
			}
		}

		Initialize(vertices, indices);
	}
	
	void Mesh::CreateCone()
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const int segments = 24;
		const float radius = 0.5f;
		const float height = 1.0f;

		// apex
		vertices.push_back({ { 0.0f, height / 2.0f, 0.0f}, {0.8f, 0.2f, 0.2f, 1.0f} });

		// base
		for(int i = 0; i <= segments; i++)
		{
			float theta = i * DirectX::XM_2PI / segments;

			vertices.push_back({ { cosf(theta) * radius, -height / 2.0f, sinf(theta) * radius}, {0.5f, 0.1f, 0.1f, 1.0f}});
		}


		for(int i = 1; i <= segments; i++)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

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
}