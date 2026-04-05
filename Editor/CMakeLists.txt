add_executable(Editor
    Source/Editor/main.cpp
    Source/Editor/Core/EditorApplication.cpp
    Source/Editor/Core/EditorApplication.hpp
    Source/Editor/Core/Project.cpp
    Source/Editor/Core/Project.hpp
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
)

target_link_libraries(Editor PRIVATE
    Engine
    imgui
    imguizmo
    tomlplusplus::tomlplusplus
)

target_include_directories(Editor PRIVATE
    Source
)

# -- Disable exceptions --
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
    target_compile_options(Engine PRIVATE -fno-exceptions)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Engine PRIVATE /EHsc-)
endif()

# -- Copy Engine assets to Editor build output --
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Engine/Assets"
    "${CMAKE_BINARY_DIR}/Editor/Assets"
)

# -- Copy SDL3 dynamic library to Editor build output --
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_BINARY_DIR}/Vendor/SDL/SDL3.dll"
    "${CMAKE_BINARY_DIR}/Editor"
)