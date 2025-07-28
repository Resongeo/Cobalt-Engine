add_executable(Editor
    Source/main.cpp
)

target_link_libraries(Editor PRIVATE
    Engine
)

target_include_directories(Editor PRIVATE
    Source
)