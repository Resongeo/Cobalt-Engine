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
    Source/Engine/Core/Types/Base.hpp
    Source/Engine/Core/Types/Containers.hpp
    Source/Engine/Core/Types/Memory.hpp
    Source/Engine/Core/Types/UUID.hpp
    Source/Engine/Core/Types/Color.hpp
    Source/Engine/Graphics/Renderer.cpp
    Source/Engine/Graphics/Renderer.hpp
    Source/Engine/Graphics/Quad.hpp
    Source/Engine/Core/Types/Math.hpp
    Source/Engine/Graphics/Shader.cpp
    Source/Engine/Graphics/Shader.hpp
    Source/Engine/Core/File.cpp
    Source/Engine/Core/File.hpp
    Source/Engine/Graphics/VertexArray.cpp
    Source/Engine/Graphics/VertexArray.hpp
    Source/Engine/Graphics/VertexBuffer.cpp
    Source/Engine/Graphics/VertexBuffer.hpp
    Source/Engine/Graphics/Attributes.hpp
    Source/Engine/Graphics/Attributes.cpp
    Source/Engine/Graphics/IndexBuffer.cpp
    Source/Engine/Graphics/IndexBuffer.hpp
    Source/Engine/Graphics/Camera.cpp
    Source/Engine/Graphics/Camera.hpp
    Source/Engine/ECS/Systems/ISystem.hpp
        Source/Engine/ECS/Systems/EditorRenderSystem.cpp
        Source/Engine/ECS/Systems/EditorRenderSystem.hpp
    Source/Engine/ECS/Systems/Schedule.hpp
    Source/Engine/Scene/SceneState.hpp
        Source/Engine/Core/Fwd.hpp
        Source/Engine/Graphics/Fwd.hpp
        Source/Engine/ECS/Components/SpriteComponent.hpp
        Source/Engine/ECS/Components/TransformComponent.hpp
)

target_link_libraries(Engine PUBLIC
    glfw
    glm::glm
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