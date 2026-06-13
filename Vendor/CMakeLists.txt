add_subdirectory(TOML)
add_subdirectory(CATCH2)
add_subdirectory(ENTT)
add_subdirectory(GLM)
add_subdirectory(SDL)
add_subdirectory(SPDLOG)

file(GLOB IMGUI_SOURCES
	"IMGUI/*.cpp"
	"IMGUI/*.h"
	"IMGUI/misc/cpp/*.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "IMGUI")
file(GLOB IMGUIZMO_SOURCES
	"IMGUIZMO/ImGuizmo.cpp"
	"IMGUIZMO/ImGuizmo.h"
)
add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})
target_link_libraries(imguizmo PRIVATE imgui)
target_include_directories(imguizmo PUBLIC "IMGUIZMO")

file(GLOB SIMDJSON_SOURCES
	"SIMDJSON/singleheader/simdjson.cpp"
	"SIMDJSON/singleheader/simdjson.h"
)
add_library(simdjson STATIC ${SIMDJSON_SOURCES})
target_include_directories(simdjson PUBLIC "SIMDJSON/singleheader")
add_subdirectory(ANGELSCRIPT/sdk/angelscript/projects/cmake)
file(GLOB ANGELSCRIPT_ADDONS
	"ANGELSCRIPT/sdk/add_on/scriptstdstring/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptstdstring/*.h"
	"ANGELSCRIPT/sdk/add_on/scriptbuilder/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptbuilder/*.h"
	"ANGELSCRIPT/sdk/add_on/weakref/*.cpp"
	"ANGELSCRIPT/sdk/add_on/weakref/*.h"
	"ANGELSCRIPT/sdk/add_on/scripthandle/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scripthandle/*.h"
	"ANGELSCRIPT/sdk/add_on/scriptmath/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptmath/*.h"
)
add_library(angelscript_addons STATIC ${ANGELSCRIPT_ADDONS})
target_link_libraries(angelscript_addons PRIVATE angelscript)
target_include_directories(angelscript_addons PUBLIC
	"ANGELSCRIPT/sdk/add_on"
)