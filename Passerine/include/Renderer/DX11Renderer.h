#pragma once

#include "Renderer/IRenderer.h"

#include <d3d11.h>

class DX11Renderer
{
public:
	DX11Renderer(HWND hWnd);
	~DX11Renderer();

	void BeginFrame(const float clearColor[4]);
	void EndFrame();

	void Resize(UINT width, UINT height);

	ID3D11Device* GetDevice() const { return m_pd3dDevice; }
	ID3D11DeviceContext* GetContext() const { return m_pd3dDeviceContext; }
	IDXGISwapChain* GetSwapChain() const { return m_pSwapChain; }

private:
	// Forward declarations of helper functions
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	bool m_SwapChainOccluded = false;
	ID3D11RenderTargetView* m_mainRenderTargetView = nullptr;
};