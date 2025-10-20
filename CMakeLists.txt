cmake_minimum_required(VERSION 3.24)
project(Cobalt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vendor libraries
add_subdirectory(Vendor/GLFW)
add_subdirectory(Vendor/TOML)
add_subdirectory(Vendor/CATCH2)
add_subdirectory(Vendor/ENTT)
add_subdirectory(Vendor/GLM)
file(GLOB IMGUI_SOURCES
    "Vendor/IMGUI/*.cpp"
    "Vendor/IMGUI/*.h"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "Vendor/IMGUI")

add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)