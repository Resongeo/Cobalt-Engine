cmake_minimum_required(VERSION 3.24)
project(Cobalt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -- Change subsystem to windows on Release --
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CMAKE_WIN32_EXECUTABLE ON)
endif ()

# -- Vendor libraries --
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
file(GLOB IMGUIZMO_SOURCES
	"Vendor/IMGUIZMO/ImGuizmo.cpp"
	"Vendor/IMGUIZMO/ImGuizmo.h"
)
add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})
target_link_libraries(imguizmo PRIVATE imgui)
target_include_directories(imguizmo PUBLIC "Vendor/IMGUIZMO")
add_subdirectory(Vendor/SDL)
file(GLOB SIMDJSON_SOURCES
	"Vendor/SIMDJSON/singleheader/simdjson.cpp"
	"Vendor/SIMDJSON/singleheader/simdjson.h"
)
add_library(simdjson STATIC ${SIMDJSON_SOURCES})
target_include_directories(simdjson PUBLIC "Vendor/SIMDJSON/singleheader")

# -- Targets --
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)
