add_library(Engine STATIC
    Source/Engine/Core/Types.hpp
    Source/Engine/Core/Application.cpp
    Source/Engine/Core/Application.hpp
    Source/Engine/Core/Logger.hpp
    Source/Engine/Core/Defines.hpp
    Source/Engine/Core/Window.hpp
    Source/Engine/Platform/Linux/LinuxWindow.cpp
)

target_link_libraries(Engine PUBLIC
    glfw
)

target_include_directories(Engine PUBLIC
    Source
)