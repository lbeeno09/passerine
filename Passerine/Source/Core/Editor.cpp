#include "Editor.h"

#include "imgui.h"

#include <tchar.h>
#include <iostream>
#include <stdexcept>
#include <string>

Editor::Editor(int width, int height, const std::wstring& title)
{
    m_window = std::make_unique<Win32Window>(width, height, title);
    m_renderer = std::make_unique<DX11Renderer>(m_window->GetHWND());
    m_imgui = std::make_unique<ImGuiManager>(m_window->GetHWND(), m_renderer->GetDevice(), m_renderer->GetContext());

    m_grid = std::make_unique<Grid>(m_renderer.get());

    m_cube = std::make_unique<Mesh>(m_renderer.get());
    m_cube->CreateCube();

    m_camera.SetPosition(0.0f, 1.0f, -5.0f);

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

Editor::~Editor() { }

void Editor::Run()
{
    // Main loop
    while(m_window->ProcessMessages())
    {
        if(m_window->WasResized())
        {
            m_renderer->Resize(m_window->GetWidth(), m_window->GetHeight());
            m_aspectRatio = static_cast<float>(m_window->GetWidth()) / static_cast<float>(m_window->GetHeight());
            m_window->ResetResizeFlag();
        }

        Render();
    }
}

void Editor::Update(float dt)
{
    m_cubeRotation += dt * 1.0f;

    if(m_viewportHovered)
    {
        ImGuiIO& io = ImGui::GetIO();
        float lookSensitivity = 0.005f;
        float moveSpeed = 5.0f;
        if(io.MouseWheel != 0.0f)
        {
            m_camera.MoveForward(io.MouseWheel * moveSpeed * 0.1f);
        }

        bool lmb = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        bool rmb = ImGui::IsMouseDown(ImGuiMouseButton_Right);
        bool mmb = ImGui::IsMouseDown(ImGuiMouseButton_Middle);
        if((lmb && rmb) || mmb)
        {
            m_camera.MoveRight(io.MouseDelta.x * dt * moveSpeed);
            m_camera.MoveUp(-io.MouseDelta.y * dt * moveSpeed);
        }
        else if(lmb)
        {
            m_camera.MoveForward(-io.MouseDelta.y * dt * moveSpeed);
            m_camera.Rotate(io.MouseDelta.x * lookSensitivity, 0.0f);
        }
        else if(rmb)
        {
            m_camera.Rotate(io.MouseDelta.x * lookSensitivity, io.MouseDelta.y * lookSensitivity);
        }
    }
    m_camera.Update();
}

void Editor::Render()
{
    m_renderer->BeginFrame();

    // Scene, render 3d cube to scene texture
    float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.0f };
    m_renderer->BindSceneRenderTarget(clearColor);

    // CB prepping
    MeshConstantBuffer cb;
    cb.view = m_camera.GetViewMatrix();
    cb.projection = m_camera.GetProjectionMatrix(m_aspectRatio);

    // Grid
    cb.world = DirectX::XMMatrixIdentity();
    m_renderer->UpdateConstantBuffer(cb);
    m_grid->Draw();
    
    // Mesh
    DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(m_cubeRotation * 0.5f);
    DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_cubeRotation);
    cb.world = rotX * rotY;
    m_renderer->UpdateConstantBuffer(cb);
    m_cube->Draw();

    // ui, render imgui to backbuffer
    m_renderer->BindMainWindowRenderTarget();
    m_imgui->BeginFrame();

    float dt = m_timer.Tick();
    Update(dt);

    ImGui::DockSpaceOverViewport();

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("Demo Window", nullptr, &m_showDemo);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(m_showDemo)
    {
        ImGui::ShowDemoWindow(&m_showDemo);
    }

    ImGui::Begin("Details");
    ImGui::Text("Transform");
    ImGui::Separator();
    static float pos[3] = { 0.0f, 0.0f, 0.0f };
    ImGui::DragFloat3("Position", pos);
    ImGui::End();

    ImGui::Begin("Content Drawer");
    ImGui::Text("Assets/Models/Textures...");
    ImGui::End();

    ImGui::Begin("Console");
    ImGui::Text("[Log] Engine Started...");
    ImGui::End();

    // viewport, handle 3d scene
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    m_viewportHovered = ImGui::IsWindowHovered();
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if(viewportSize.x > 0 && viewportSize.y > 0)
    {
        float currentAR = viewportSize.x / viewportSize.y;
        if(currentAR != m_aspectRatio)
        {
            m_aspectRatio = currentAR;
        }
    }
    ImGui::Image((void*)m_renderer->GetSceneSRV(), viewportSize);
    ImGui::End();
    ImGui::PopStyleVar();

    m_imgui->EndFrame();

    m_renderer->EndFrame();
}
