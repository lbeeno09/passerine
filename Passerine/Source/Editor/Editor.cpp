#include "Editor/Editor.h"

#include "Engine/Core/Input.h"

#include "imgui.h"

#include <tchar.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Editor
{
    Editor::Editor(int width, int height, const std::wstring& title)
    {
        m_window = std::make_unique<Engine::Win32Window>(width, height, title);
        m_renderer = std::make_unique<Engine::DX11Renderer>(m_window->GetHWND());
        m_imgui = std::make_unique<ImGuiManager>(m_window->GetHWND(), m_renderer->GetDevice(), m_renderer->GetContext());

        m_grid = std::make_unique<Engine::Grid>(m_renderer.get());

        //m_cube = std::make_unique<Engine::Mesh>(m_renderer.get());
        auto cube = std::make_unique<Engine::Mesh>(m_renderer.get(), GetNextAvailableName("Cube"));
        cube->CreateCube();
        m_sceneEntities.push_back(std::move(cube));


        m_camera = std::make_unique<Engine::Camera>();
        m_camera->SetPosition(0.0f, 1.0f, -5.0f);

        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }

    Editor::~Editor() {}

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
        if(m_isNavigating)
        {
            ImGuiIO& io = ImGui::GetIO();
            float scrollSpeed = 5.0f;
            if(io.MouseWheel != 0.0f)
            {
                m_camera->MoveForward(io.MouseWheel * scrollSpeed * 0.1f);
            }
        }

        bool lmb = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        bool rmb = ImGui::IsMouseDown(ImGuiMouseButton_Right);
        bool mmb = ImGui::IsMouseDown(ImGuiMouseButton_Middle);

        // Chekck if any navigation button is made
        if((lmb || rmb || mmb) && m_viewportHovered && !m_isNavigating)
        {
            m_isNavigating = true;
            m_window->CaptureMouse();

            Engine::Input::GetMousePosition(m_anchorX, m_anchorY);
        }

        // infinite drag loop
        if(m_isNavigating)
        {
            int curX, curY;
            Engine::Input::GetMousePosition(curX, curY);

            float dx = (float)(curX - m_anchorX);
            float dy = (float)(curY - m_anchorY);
            if(dx != 0 || dy != 0)
            {
                float lookSensitivity = 0.005f;
                float moveSpeed = 5.0f;

                if((lmb && rmb) || mmb)
                {
                    m_camera->MoveRight(dx * dt * moveSpeed);
                    m_camera->MoveUp(-dy * dt * moveSpeed);
                }
                else if(lmb)
                {
                    m_camera->MoveForward(-dy * dt * moveSpeed);
                    m_camera->Rotate(dx * lookSensitivity, 0.0f);
                }
                else if(rmb)
                {
                    m_camera->Rotate(dx * lookSensitivity, dy * lookSensitivity);
                }

                m_window->SetMousePosition(m_anchorX, m_anchorY);
            }

            if(!(lmb || rmb || mmb))
            {
                m_isNavigating = false;
                m_window->ReleaseMouse();
            }
        }

        m_camera->Update();
    }

    void Editor::Render()
    {
        m_renderer->BeginFrame();

        // Scene, render 3d cube to scene texture
        float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.0f };
        m_renderer->BindSceneRenderTarget(clearColor);

        // CB prepping
        Engine::MeshConstantBuffer cb;
        cb.view = m_camera->GetViewMatrix();
        cb.projection = m_camera->GetProjectionMatrix(m_aspectRatio);

        // Grid
        cb.world = DirectX::XMMatrixIdentity();
        m_renderer->UpdateConstantBuffer(cb);
        m_grid->Draw();

        // Meshes
        for(auto& entity : m_sceneEntities)
        {
            cb.world = entity->GetWorldMatrix();
            m_renderer->UpdateConstantBuffer(cb);
            entity->Draw();
        }

        // ui, render imgui to backbuffer
        m_renderer->BindMainWindowRenderTarget();
        m_imgui->BeginFrame();

        float dt = m_timer.Tick();
        Update(dt);

        RenderUI();

        m_imgui->EndFrame();
        m_renderer->EndFrame();
    }

    void Editor::RenderUI()
    {
        ImGui::DockSpaceOverViewport();

        DrawMainMenuBar();
        DrawViewport();
        DrawOutliner();
        DrawDetails();
        DrawContentDrawer();
        DrawConsole();

    }

    void Editor::DrawMainMenuBar()
    {
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
    }

    void Editor::DrawViewport()
    {
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
    }

    void Editor::DrawOutliner()
    {
        ImGui::Begin("Outliner");
        if(ImGui::Button("+ Add..."))
        {
            ImGui::OpenPopup("AddPrimitivePopup");
        }
        if(ImGui::BeginPopup("AddPrimitivePopup"))
        {
            if(ImGui::MenuItem("Cube"))
            {
                SpawnPrimitive("Cube");
            }
            if(ImGui::MenuItem("Plane"))
            {
                SpawnPrimitive("Plane");
            }
            if(ImGui::MenuItem("Sphere"))
            {
                SpawnPrimitive("Sphere");
            }
            if(ImGui::MenuItem("Cyllinder"))
            {
                SpawnPrimitive("Cyllinder");
            }
            if(ImGui::MenuItem("Capsule"))
            {
                SpawnPrimitive("Capsule");
            }
            if(ImGui::MenuItem("Cone"))
            {
                SpawnPrimitive("Cone");
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if(ImGui::Button("- Delete") && m_selectedEntity)
        {
            auto it = std::find_if(m_sceneEntities.begin(), m_sceneEntities.end(), [&](const std::unique_ptr<Engine::Entity>& e) { return e.get() == m_selectedEntity; });
            if(it != m_sceneEntities.end())
            {
                m_sceneEntities.erase(it);
                m_selectedEntity = nullptr;
            }
        }
        ImGui::Separator();

        for(auto& entity : m_sceneEntities)
        {
            bool isSelected = (m_selectedEntity == entity.get());

            // renaming
            if(m_renamingEntity == entity.get())
            {
                ImGui::SetKeyboardFocusHere();

                if(ImGui::InputText("##InPlaceRename", m_renameBuffer, sizeof(m_renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if(strlen(m_renameBuffer) > 0)
                    {
                        entity->SetName(m_renameBuffer);
                    }
                    m_renamingEntity = nullptr;
                }
            }
            else // regular display
            {
                if(ImGui::Selectable(entity->GetName().c_str(), isSelected))
                {
                    m_selectedEntity = entity.get();
                }

                if(isSelected && ImGui::IsMouseDoubleClicked(0))
                {
                    m_renamingEntity = entity.get();
                    strcpy_s(m_renameBuffer, entity->GetName().c_str());
                }
            }
        }
        ImGui::End();
    }

    void Editor::DrawDetails()
    {
        ImGui::Begin("Details");
        if(m_selectedEntity)
        {
            ImGui::Text("Selection: %s", m_selectedEntity->GetName().c_str());
            ImGui::Separator();

            auto& pos = m_selectedEntity->GetPosition();
            auto& rot = m_selectedEntity->GetRotation();
            auto& scl = m_selectedEntity->GetScale();

            ImGui::DragFloat3("Position", &pos.x, 0.1f);
            ImGui::SameLine();
            if(ImGui::Button("R##Pos"))
            {
                pos = { 0, 0, 0 };
            }

            ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
            ImGui::SameLine();
            if(ImGui::Button("R##Rot"))
            {
                rot = { 0, 0, 0 };
            }

            ImGui::DragFloat3("Scale", &scl.x, 0.1f);
            ImGui::SameLine();
            if(ImGui::Button("R##Scl"))
            {
                scl = { 1, 1, 1 };
            }
        }
        ImGui::End();
    }

    void Editor::DrawContentDrawer()
    {
        ImGui::Begin("Content Drawer");
        ImGui::Text("Assets/Models/Textures...");
        ImGui::End();
    }

    void Editor::DrawConsole()
    {
        ImGui::Begin("Console");
        ImGui::Text("[Log] Engine Started...");
        ImGui::End();
    }

    void Editor::SpawnPrimitive(const std::string& type)
    {
        std::string smartName = GetNextAvailableName(type);
        auto entity = std::make_unique<Engine::Mesh>(m_renderer.get(), smartName);

        if(type == "Cube")
        {
            entity->CreateCube();

        }
        if(type == "Plane")
        {
            entity->CreatePlane();
        }
        if(type == "Sphere")
        {
            entity->CreateSphere();
        }
        if(type == "Cyllinder")
        {
            entity->CreateCyllinder();
        }
        if(type == "Capsule")
        {
            entity->CreateCapsule();
        }
        if(type == "Cone")
        {
            entity->CreateCone();
        }

        m_sceneEntities.push_back(std::move(entity));
    }

    std::string Editor::GetNextAvailableName(const std::string& prefix)
    {
        int index = 1;
        while(true)
        {
            char buffer[32];
            sprintf_s(buffer, "%s%02d", prefix.c_str(), index);
            std::string candidate = buffer;

            bool exists = false;
            for(const auto& entity : m_sceneEntities)
            {
                if(entity->GetName() == candidate)
                {
                    exists = true;
                    break;
                }
            }

            if(!exists)
            {
                return candidate;
            }
            index++;
        }
    }
}