#include "DX11Renderer.h"

#include <d3dcompiler.h>
#include <stdexcept>

namespace Engine
{
    DX11Renderer::DX11Renderer(HWND hWnd)
    {
        if(!CreateDeviceD3D(hWnd))
        {
            throw std::runtime_error("ERROR: Init DX11 Failed");
        }

        CreateShaders();
        CreatePipelineStates();

        RECT rect;
        GetClientRect(hWnd, &rect);
        Resize(rect.right - rect.left, rect.bottom - rect.top);

        //CreateSceneResource(rect.right - rect.left, rect.bottom - rect.top);
        //CreateDepthStencil(rect.right - rect.left, rect.bottom - rect.top);
    }

    void DX11Renderer::BeginFrame()
    {
        m_context->ClearState();
    }

    void DX11Renderer::EndFrame()
    {
        // Present
        HRESULT hr = m_swapChain->Present(1, 0);   // Present with vsync
        m_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    void DX11Renderer::BindSceneRenderTarget(const float clearColor[4])
    {
        // Redirect output to Scene texture
        m_context->OMSetRenderTargets(1, m_sceneRTV.GetAddressOf(), m_depthStencilView.Get());
        m_context->ClearRenderTargetView(m_sceneRTV.Get(), clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        m_context->OMSetDepthStencilState(m_depthState.Get(), 1);
        m_context->RSSetState(m_rasterState.Get());

        // setup 3d pipeline
        m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
        m_context->IASetInputLayout(m_inputLayout.Get());
        m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        // Set viewport
        D3D11_TEXTURE2D_DESC desc;
        m_sceneTexture->GetDesc(&desc);
        D3D11_VIEWPORT  vp = { 0.0f, 0.0f, (float)desc.Width, (float)desc.Height, 0.0f, 1.0f };
        m_context->RSSetViewports(1, &vp);
    }

    void DX11Renderer::BindMainWindowRenderTarget()
    {
        // switch to backbuffer
        float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

        m_context->OMSetRenderTargets(1, m_mainRTV.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_mainRTV.Get(), clearColor);
    }

    void DX11Renderer::Resize(UINT width, UINT height)
    {
        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if(width == 0 || height == 0)
        {
            return;
        }

        //// Handle window being minimized or screen locked
        //if(m_SwapChainOccluded && m_swapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        //{
        //    ::Sleep(10);
        //}
        //m_SwapChainOccluded = false;

        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_mainRTV.Reset();
        m_sceneRTV.Reset();
        m_sceneSRV.Reset();
        m_sceneTexture.Reset();
        m_depthStencilView.Reset();
        m_depthStencilTexture.Reset();
        m_context->Flush();

        HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        if(FAILED(hr))
        {
            return;
        }

        CreateRenderTarget();
        CreateSceneResource(width, height);
        CreateDepthStencil(width, height);

        D3D11_VIEWPORT vp = { 0, 0, (float)width, (float)height, 0.0f, 1.0f };
        m_context->RSSetViewports(1, &vp);
    }

    void DX11Renderer::UpdateConstantBuffer(const MeshConstantBuffer& data)
    {
        MeshConstantBuffer transposedData;
        transposedData.world = DirectX::XMMatrixTranspose(data.world);
        transposedData.view = DirectX::XMMatrixTranspose(data.view);
        transposedData.projection = DirectX::XMMatrixTranspose(data.projection);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if(SUCCEEDED(hr))
        {
            memcpy(mappedResource.pData, &transposedData, sizeof(MeshConstantBuffer));
            m_context->Unmap(m_constantBuffer.Get(), 0);
        }
    }

    // Helper functions
    bool DX11Renderer::CreateDeviceD3D(HWND hWnd)
    {
        // Setup swap chain
        // This is a basic setup. Optimally could use e.g. DXGI_SWAP_EFFECT_FLIP_DISCARD and handle fullscreen mode differently. See #8979 for suggestions.
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.Windowed = TRUE;

        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
        };

        HRESULT res = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd,
            m_swapChain.GetAddressOf(), m_device.GetAddressOf(), &featureLevel, m_context.GetAddressOf()
        );
        if(res != S_OK)
        {
            return false;
        }

        // Create Context Buffer
        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.ByteWidth = sizeof(MeshConstantBuffer);
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        m_device->CreateBuffer(&cbd, nullptr, m_constantBuffer.GetAddressOf());

        // Disable DXGI's default Alt+Enter fullscreen behavior.
        // - You are free to leave this enabled, but it will not work properly with multiple viewports.
        // - This must be done for all windows associated to the device. Our DX11 backend does this automatically for secondary viewports that it creates.
        Microsoft::WRL::ComPtr<IDXGIFactory> pSwapChainFactory;
        if(SUCCEEDED(m_swapChain->GetParent(IID_PPV_ARGS(pSwapChainFactory.GetAddressOf()))))
        {
            pSwapChainFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
        }

        CreateRenderTarget();
        return true;
    }

    void DX11Renderer::CreateRenderTarget()
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;

        HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if(FAILED(hr))
        {
            return;
        }

        hr = m_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_mainRTV.GetAddressOf());
        if(FAILED(hr))
        {
            return;
        }
    }

    void DX11Renderer::CreateSceneResource(UINT width, UINT height)
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = width;
        td.Height = height;
        td.MipLevels = 2;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        m_device->CreateTexture2D(&td, nullptr, m_sceneTexture.ReleaseAndGetAddressOf());
        m_device->CreateRenderTargetView(m_sceneTexture.Get(), nullptr, m_sceneRTV.ReleaseAndGetAddressOf());
        m_device->CreateShaderResourceView(m_sceneTexture.Get(), nullptr, m_sceneSRV.ReleaseAndGetAddressOf());
    }

    void DX11Renderer::CreateDepthStencil(UINT width, UINT height)
    {
        D3D11_TEXTURE2D_DESC dtd = {};
        dtd.Width = width;
        dtd.Height = height;
        dtd.MipLevels = 1;
        dtd.ArraySize = 1;
        dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dtd.SampleDesc.Count = 1;
        dtd.Usage = D3D11_USAGE_DEFAULT;
        dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        m_device->CreateTexture2D(&dtd, nullptr, m_depthStencilTexture.ReleaseAndGetAddressOf());
        m_device->CreateDepthStencilView(m_depthStencilTexture.Get(), nullptr, m_depthStencilView.ReleaseAndGetAddressOf());
    }

    void DX11Renderer::CreateShaders()
    {
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        HRESULT hr = D3DCompileFromFile(
            L"Assets/Shaders/Shader.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VSMain", "vs_5_0",
            0, 0, &vsBlob, &errorBlob
        );
        if(FAILED(hr))
        {
            if(errorBlob)
            {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            }
            throw std::exception("Vertex Shader Compilation Failed");
        }
        m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        hr = m_device->CreateInputLayout(
            layout,
            ARRAYSIZE(layout),
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            &m_inputLayout
        );
        if(FAILED(hr))
        {
            throw std::exception("Input Layout Create Failed");
        }

        hr = D3DCompileFromFile(
            L"Assets/Shaders/Shader.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PSMain", "ps_5_0",
            0, 0, &psBlob, &errorBlob
        );
        if(FAILED(hr))
        {
            if(errorBlob)
            {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            }
            throw std::exception("Pixel Shader Compilation Failed");
        }
        m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

    }

    void DX11Renderer::CreatePipelineStates()
    {
        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        m_device->CreateDepthStencilState(&dsDesc, &m_depthState);

        D3D11_RASTERIZER_DESC rsDesc = {};
        rsDesc.FillMode = D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_NONE;
        rsDesc.FrontCounterClockwise = FALSE;
        rsDesc.DepthClipEnable = TRUE;
        m_device->CreateRasterizerState(&rsDesc, &m_rasterState);
    }
}