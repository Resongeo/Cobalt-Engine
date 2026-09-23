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