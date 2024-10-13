cmake_minimum_required(VERSION 3.10)

# Project name
project(Pascua VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# path to vcpkg
set(CMAKE_TOOLCHAIN_FILE "%VCPKG_ROOT%\\scripts\\buildsystems\\vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows)
set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}")
list(APPEND CMAKE_PREFIX_PATH ${VCPKG_INSTALLED_DIR})

find_package(Vulkan REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Freetype REQUIRED)

# Set Include Directories
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add header files for the app
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(Pascua ${SOURCES})

# Link libraries
target_link_libraries(Pascua PRIVATE Vulkan::Vulkan imgui::imgui Freetype::Freetype)

# set output directory
set_target_properties(Pascua PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)