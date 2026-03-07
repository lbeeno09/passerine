#include "Core/App.h"

#include "imgui.h"

#include <tchar.h>
#include <iostream>
#include <stdexcept>
#include <string>

App::App(int width, int height, const std::wstring& title)
{
    m_window = std::make_unique<Win32Window>(width, height, title);
    m_renderer = std::make_unique<DX11Renderer>(m_window->GetHWND());
    m_imgui = std::make_unique<ImGuiManager>(m_window->GetHWND(), m_renderer->GetDevice(), m_renderer->GetContext());
}

App::~App() { }

void App::Run()
{
    // Main loop
    while(m_window->ProcessMessages())
    {
        float dt = m_timer.Tick();
        
        if(m_window->WasResized())
        {
            m_renderer->Resize(m_window->GetWidth(), m_window->GetHeight());
            m_window->ResetResizeFlag();
        }

        Update(dt);
        Render();
    }
}

void App::Update(float dt)
{

}

void App::Render()
{
    float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.00f };

    m_renderer->BeginFrame(clearColor);

    m_imgui->BeginFrame();

    DrawUI();

    m_imgui->EndFrame();
    m_renderer->EndFrame();
}

void App::DrawUI()
{
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

    ImGui::Begin("Viewport");
    ImGui::Text("3D Scene will render here.");
    ImGui::End();
}
