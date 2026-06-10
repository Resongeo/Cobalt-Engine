add_executable(Editor
	Source/Editor/main.cpp
	Source/Editor/Core/EditorApplication.cpp
	Source/Editor/Core/EditorApplication.hpp
	Source/Editor/Gui/ImGuiBuild.cpp
	Source/Editor/Gui/Gui.cpp
	Source/Editor/Gui/Gui.hpp
	Source/Editor/Gui/IPanel.hpp
	Source/Editor/Core/EditorState.hpp
	Source/Editor/Gui/Panels/SceneHierarchyPanel.cpp
	Source/Editor/Gui/Panels/SceneHierarchyPanel.hpp
	Source/Editor/Gui/Panels/ViewportPanel.cpp
	Source/Editor/Gui/Panels/ViewportPanel.hpp
	Source/Editor/Gui/Panels/EntityComponentsPanel.cpp
	Source/Editor/Gui/Panels/EntityComponentsPanel.hpp
	Source/Editor/Gui/Panels/AssetBrowserPanel.cpp
	Source/Editor/Gui/Panels/AssetBrowserPanel.hpp
	Source/Editor/Gui/Fonts.hpp
	Source/Editor/Gui/Widgets.hpp
	Source/Editor/Gui/Widgets.cpp
	Source/Editor/Gui/Helpers.hpp
	Source/Editor/Gui/Colors.hpp
	Source/Editor/Gui/Panels/LogPanel.cpp
	Source/Editor/Gui/Panels/LogPanel.hpp
)

target_link_libraries(Editor PRIVATE
	Engine
	imgui
	imguizmo
)

target_include_directories(Editor PRIVATE
	Source
)

target_compile_definitions(Editor PRIVATE
	$<$<CONFIG:Debug>:CONFIG_DEBUG>
	$<$<CONFIG:Release>:CONFIG_RELEASE>
	$<$<CONFIG:MinSizeRel>:CONFIG_RELEASE>
)

if (WIN32)
	set_target_properties(Editor PROPERTIES
		WIN32_EXECUTABLE $<$<CONFIG:Release>:ON>
		WIN32_EXECUTABLE $<$<CONFIG:MinSizeRel>:ON>
	)
endif ()

# -- Copy Engine assets to Editor build output --
add_custom_command(TARGET Editor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Engine/Assets"
	"${CMAKE_BINARY_DIR}/Editor/Assets"
)

# -- Copy SDL3 dynamic library to Editor build output --
add_custom_command(TARGET Editor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:SDL3::SDL3>
	$<TARGET_FILE_DIR:Editor>
)
