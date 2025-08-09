add_executable(Editor
    Source/Editor/main.cpp
    Source/Editor/Core/EditorApplication.cpp
    Source/Editor/Core/EditorApplication.hpp
    Source/Editor/Core/Project.cpp
    Source/Editor/Core/Project.hpp
)

target_link_libraries(Editor PRIVATE
    Engine
    tomlplusplus::tomlplusplus
)

target_include_directories(Editor PRIVATE
    Source
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
    target_compile_options(Engine PRIVATE -fno-exceptions)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Engine PRIVATE /EHsc-)
endif()