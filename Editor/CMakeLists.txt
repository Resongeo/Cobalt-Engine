add_executable(Editor
    Source/Editor/main.cpp
)

target_link_libraries(Editor PRIVATE
    Engine
)

target_include_directories(Editor PRIVATE
    Source/Editor
)