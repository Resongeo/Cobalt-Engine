cmake_minimum_required(VERSION 3.24)
project(Cobalt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM_MASM)

# -- Change subsystem to windows on Release --
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
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
	"Vendor/IMGUI/misc/cpp/*.cpp"
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
add_subdirectory(Vendor/ANGELSCRIPT/sdk/angelscript/projects/cmake)
file(GLOB ANGELSCRIPT_ADDONS
	"Vendor/ANGELSCRIPT/sdk/add_on/scriptstdstring/*.cpp"
	"Vendor/ANGELSCRIPT/sdk/add_on/scriptstdstring/*.h"
	"Vendor/ANGELSCRIPT/sdk/add_on/scriptbuilder/*.cpp"
	"Vendor/ANGELSCRIPT/sdk/add_on/scriptbuilder/*.h"
	"Vendor/ANGELSCRIPT/sdk/add_on/weakref/*.cpp"
	"Vendor/ANGELSCRIPT/sdk/add_on/weakref/*.h"
	"Vendor/ANGELSCRIPT/sdk/add_on/scripthandle/*.cpp"
	"Vendor/ANGELSCRIPT/sdk/add_on/scripthandle/*.h"
)
add_library(angelscript_addons STATIC ${ANGELSCRIPT_ADDONS})
target_link_libraries(angelscript_addons PRIVATE angelscript)
target_include_directories(angelscript_addons PUBLIC
	"Vendor/ANGELSCRIPT/sdk/add_on"
)

# -- Targets --
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)
