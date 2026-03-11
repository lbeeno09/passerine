#pragma once

#include "Graphics/GraphicsTypes.h"
#include <d3d11.h>
#include <wrl/client.h>

class DX11Renderer
{
public:
	DX11Renderer(HWND hWnd);
	~DX11Renderer();

	void BeginFrame();
	void EndFrame();
	void Resize(UINT width, UINT height);

	// Target Switching
	void BindSceneRenderTarget(const float clearColor[4]);
	void BindMainWindowRenderTarget();
	void UpdateConstantBuffer(const MeshConstantBuffer& data);

	ID3D11Device* GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() const { return m_context.Get(); }
	IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }
	ID3D11ShaderResourceView* GetSceneSRV() const { return m_sceneSRV.Get(); }

private:
	bool CreateDeviceD3D(HWND hWnd);
	void CreateRenderTarget();
	void CreateSceneResource(UINT width, UINT height);
	void CreateDepthStencil(UINT width, UINT height);
	void CreateShaders();
	void CreatePipelineStates();

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_mainRenderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_sceneTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_sceneRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_sceneSRV;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthState;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	bool m_SwapChainOccluded = false;

};