add_executable(Editor
    Source/Editor/main.cpp
    Source/Editor/Core/EditorApplication.cpp
    Source/Editor/Core/EditorApplication.hpp
)

target_link_libraries(Editor PRIVATE
    Engine
)

target_include_directories(Editor PRIVATE
    Source
)