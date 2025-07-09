# Folder Structure Idea

passerine
 ├─ .git/
 ├─ build/ (CMake output)
 ├─ bin/ (Output for executable)
 ├─ lib/ (Output for compiled libs)
 ├─ Assets/ (arts, models, shader, sounds, etc...)
 │  ├─ Models/
 │  ├─ Textures/
 │  ├─ Shaders/
 │  └─ ...
 ├─ Vendor/ (External libraries; maybe Git Submodules?)
 │  ├─ glfw/
 │  ├─ glm/
 │  ├─ glad/
 │  ├─ imgui/
 │  ├─ stb_image/
 │  └─ ...
 ├─ Src/
 │  ├─ Core/
 │  ├─ Graphics/
 │  ├─ Physics/
 │  ├─ Platform/
 │  └─ CMakeLists.txt
 ├─ Sandbox/
 │  ├─ SandboxApp.cpp
 │  └─ CMakeLists.txt
 │
 ├─ CMakeLists.txt
 └─ README.me