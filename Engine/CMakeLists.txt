add_library(Engine STATIC
    Source/Engine/Core/Types.hpp
    Source/Engine/Core/Application.cpp
    Source/Engine/Core/Application.hpp
    Source/Engine/Core/Logger.hpp
    Source/Engine/Core/Defines.hpp
    Source/Engine/Core/Window.hpp
    Source/Engine/Platform/Linux/LinuxWindow.cpp
    Source/Engine/Platform/Windows/WindowsWindow.cpp
        Source/Engine/Platform/Platform.hpp
)

target_link_libraries(Engine PUBLIC
    glfw
)

target_include_directories(Engine PUBLIC
    Source
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
    target_compile_options(Engine PRIVATE -fno-exceptions)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Engine PRIVATE /EHsc-)
endif()