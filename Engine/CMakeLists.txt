add_library(Engine STATIC
    Source/Engine/Core/Types.hpp
    Source/Engine/Core/Application.cpp
    Source/Engine/Core/Application.hpp
    Source/Engine/Core/Logger.hpp
    Source/Engine/Core/Defines.hpp
    Source/Engine/Core/Window.hpp
    Source/Engine/Platform/Linux/LinuxWindow.cpp
    Source/Engine/Platform/Windows/WindowsWindow.cpp
    Source/Engine/Platform/Platform.cpp
    Source/Engine/Platform/Platform.hpp
    Source/Engine/Scene/SceneManager.cpp
    Source/Engine/Scene/SceneManager.hpp
    Source/Engine/Scene/Scene.hpp
        Source/Engine/ECS/Components/TagComponent.hpp
        Source/Engine/Scene/Scene.cpp
)

target_link_libraries(Engine PUBLIC
    glfw
    EnTT::EnTT
)

target_include_directories(Engine PUBLIC
    Source
    ${CMAKE_SOURCE_DIR}/Vendor/GLAD
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
    target_compile_options(Engine PRIVATE -fno-exceptions)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Engine PRIVATE /EHsc-)
endif()